package ru.frostman.study.yrin.fifth;

import ru.frostman.study.yrin.fifth.model.BaseEntry;
import ru.frostman.study.yrin.fifth.model.DirEntry;
import ru.frostman.study.yrin.fifth.model.FileEntry;
import ru.frostman.study.yrin.fifth.model.StoredFile;
import ru.frostman.study.yrin.fifth.props.AppMode;
import ru.frostman.study.yrin.fifth.props.AppProperties;
import ru.frostman.study.yrin.fifth.props.GetOpts;
import ru.frostman.study.yrin.fifth.util.DirectoryScanner;
import ru.frostman.study.yrin.fifth.util.HashCodeCalculator;
import ru.frostman.study.yrin.fifth.util.XMLUtil;

import java.io.File;
import java.util.Map;

/**
 * @author slukjanov aka Frostman
 */
public class Main {
    public static void main(String[] args) {
        AppProperties props = GetOpts.parse(args);

        if (props.getMode() == AppMode.DIFF) {
            diff(props.getStorageFile(), props.getRootDir(), false);
        } else if (props.getMode() == AppMode.SCAN) {
            scan(props.getStorageFile(), props.getRootDir(), true);
        } else if (props.getMode() == AppMode.DIFF_UPDATE) {
            diff(props.getStorageFile(), props.getRootDir(), true);
        }
    }

    private static DirEntry scan(File storage, File root, boolean update) {
        DirectoryScanner scanner = new DirectoryScanner();
        DirEntry entry = new DirEntry();
        scanner.scan(root, entry);
        HashCodeCalculator.getInstance().waitFor();

        if (update) {
            XMLUtil.writeToXML(entry, storage);
        }

        return entry;
    }

    private static void diff(File storage, File root, boolean update) {
        Map<File, StoredFile> files = XMLUtil.readStoredFiles(storage);
        DirEntry dirEntry = scan(storage, root, update);

        printDiff(files, dirEntry, null);
        printRemovedFiles(files);
    }

    private static void printDiff(Map<File, StoredFile> files, BaseEntry root, File path) {
        if (root instanceof FileEntry && path != null) {
            FileEntry file = (FileEntry) root;
            String hash = file.getHash();

            StoredFile storedFile = files.get(path.getAbsoluteFile());
            if (storedFile == null) {
                System.out.println("File '" + path.getAbsolutePath() + "' created:\n\t new hash: " + hash);
            } else {
                String storedHash = storedFile.getHash();

                storedFile.setChecked(true);

                if (!hash.equals(storedHash)) {
                    System.out.println("File '" + path.getAbsolutePath() + "' changed:\n\t old hash: "
                            + storedHash + "\n\t new hash: " + hash);
                }
            }
        } else if (root instanceof DirEntry) {
            for (Map.Entry<File, BaseEntry> pair : ((DirEntry) root).getEntries().entrySet()) {
                printDiff(files, pair.getValue(), pair.getKey());
            }
        }
    }

    private static void printRemovedFiles(Map<File, StoredFile> files) {
        for (Map.Entry<File, StoredFile> pair : files.entrySet()) {
            StoredFile file = pair.getValue();

            if (!file.isChecked()) {
                System.out.println("File '" + pair.getKey().getAbsolutePath() + "' removed");
            }
        }
    }

    // scanner scan test.xml C:/temp
    // scanner diff test.xml C:/temp
}

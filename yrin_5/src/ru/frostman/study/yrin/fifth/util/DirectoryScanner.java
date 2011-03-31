package ru.frostman.study.yrin.fifth.util;

import ru.frostman.study.yrin.fifth.model.BaseEntry;
import ru.frostman.study.yrin.fifth.model.DirEntry;
import ru.frostman.study.yrin.fifth.model.FileEntry;
import ru.frostman.study.yrin.fifth.model.FileHashJob;

import java.io.File;
import java.io.FileFilter;

/**
 * @author slukjanov aka Frostman
 */
public class DirectoryScanner {

    public DirectoryScanner() {
    }

    public void scan(File root, BaseEntry entry) {
        if (root.isDirectory() && !(entry instanceof DirEntry)) {
            throw new IllegalArgumentException("Entry must be DirEntry iff dir used as root");
        }

        if (root.isFile() && !(entry instanceof FileEntry)) {
            throw new IllegalArgumentException("Entry must be FileEntry iff file used as root");
        }

        if (root.isFile()) {
            HashCodeCalculator.getInstance().calc(new FileHashJob(root, (FileEntry) entry));
        } else if (root.isDirectory()) {
            DirEntry rootDir = (DirEntry) entry;

            FileFilter fileFilter = new FileFilter() {
                public boolean accept(File file) {
                    return file.canRead();
                }
            };

            File[] fileArray = root.listFiles(fileFilter);

            if (fileArray == null) {
                return;
            }

            for (File file : fileArray) {
                BaseEntry tmp;
                if (file.isDirectory()) {
                    tmp = new DirEntry();
                    scan(file, tmp);
                } else {
                    tmp = new FileEntry();
                    scan(file, tmp);
                }
                rootDir.put(file, tmp);
            }
        }
    }
}

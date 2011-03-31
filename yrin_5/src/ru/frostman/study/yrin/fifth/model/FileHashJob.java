package ru.frostman.study.yrin.fifth.model;

import ru.frostman.study.yrin.fifth.model.FileEntry;

import java.io.File;

/**
 * @author slukjanov aka Frostman
 */
public class FileHashJob {
    private final File file;
    private final FileEntry entry;

    public FileHashJob(File file, FileEntry entry) {
        this.file = file;
        this.entry = entry;
    }

    public File getFile() {
        return file;
    }

    public FileEntry getEntry() {
        return entry;
    }
}

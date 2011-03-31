package ru.frostman.study.yrin.fifth.model;

import ru.frostman.study.yrin.fifth.model.BaseEntry;

/**
 * @author slukjanov aka Frostman
 */
public class FileEntry implements BaseEntry {
    private String hash;

    public FileEntry() {
    }

    public void setHash(String hash) {
        this.hash = hash;
    }

    public String getHash() {
        return hash;
    }
}

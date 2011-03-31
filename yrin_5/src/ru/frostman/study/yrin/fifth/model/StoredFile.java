package ru.frostman.study.yrin.fifth.model;

import java.io.File;

/**
 * @author slukjanov aka Frostman
 */
public class StoredFile {
    private File file;
    private String hash;
    private boolean checked;

    public StoredFile() {
    }

    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }

    public String getHash() {
        return hash;
    }

    public void setHash(String hash) {
        this.hash = hash;
    }

    public boolean isChecked() {
        return checked;
    }

    public void setChecked(boolean checked) {
        this.checked = checked;
    }
}

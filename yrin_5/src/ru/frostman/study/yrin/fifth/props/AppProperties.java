package ru.frostman.study.yrin.fifth.props;

import java.io.File;

/**
 * @author slukjanov aka Frostman
 */
public class AppProperties {
    private AppMode mode;
    private File storageFile;
    private File rootDir;

    private boolean valid;

    public AppProperties() {
    }

    public AppMode getMode() {
        return mode;
    }

    public void setMode(AppMode mode) {
        this.mode = mode;
    }

    public File getStorageFile() {
        return storageFile;
    }

    public void setStorageFile(File storageFile) {
        this.storageFile = storageFile;
    }

    public File getRootDir() {
        return rootDir;
    }

    public void setRootDir(File rootDir) {
        this.rootDir = rootDir;
    }

    public boolean isValid() {
        return valid;
    }

    public void setValid(boolean valid) {
        this.valid = valid;
    }
}

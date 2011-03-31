package ru.frostman.study.yrin.fifth.model;

import ru.frostman.study.yrin.fifth.model.BaseEntry;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

/**
 * @author slukjanov aka Frostman
 */
public class DirEntry implements BaseEntry{
    private final Map<File, BaseEntry> entries = new HashMap<File, BaseEntry>();

    public DirEntry() {
    }

    public Map<File, BaseEntry> getEntries() {
        return entries;
    }

    public void put(File file, BaseEntry entry) {
        entries.put(file, entry);
    }
}

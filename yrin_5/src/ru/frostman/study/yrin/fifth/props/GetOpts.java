package ru.frostman.study.yrin.fifth.props;

import java.io.File;

/**
 * @author slukjanov aka Frostman
 */
public class GetOpts {
    public static AppProperties parse(String[] args) {
        AppProperties props = new AppProperties();

        props.setMode(AppMode.valueOf(args[0].toUpperCase()));
        props.setStorageFile(new File(args[1]));

        StringBuilder sb = new StringBuilder();
        for (int i = 2; i < args.length; i++) {
            sb.append(args[i]).append(' ');
        }

        props.setRootDir(new File(sb.toString().trim()));

        return props;
    }
}

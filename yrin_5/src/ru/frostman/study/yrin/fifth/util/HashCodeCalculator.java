package ru.frostman.study.yrin.fifth.util;

import ru.frostman.study.yrin.fifth.model.FileHashJob;

import java.io.FileInputStream;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author slukjanov aka Frostman
 */
public class HashCodeCalculator {
    private static final int BUFFER_SIZE = 1024;

    private static final HashCodeCalculator INSTANCE = new HashCodeCalculator();

    private final BlockingQueue<FileHashJob> jobs = new LinkedBlockingQueue<FileHashJob>();
    private final AtomicInteger jobsCounter = new AtomicInteger(0);
    private final Object monitor = new Object();

    public HashCodeCalculator() {
        int workers = 3;

        try {
            for (int i = 0; i < workers; i++) {
                Thread t = new Thread(new Worker(MessageDigest.getInstance("SHA-1")));
                t.setDaemon(true);
                t.start();
            }
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }

    public static HashCodeCalculator getInstance() {
        return INSTANCE;
    }

    public void calc(FileHashJob job) {
        jobs.add(job);
        jobsCounter.incrementAndGet();
    }

    public void waitFor() {
        while (jobsCounter.get() > 0) {
            synchronized (monitor) {
                try {
                    monitor.wait(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private final class Worker implements Runnable {
        private final MessageDigest md;
        private boolean work = true;

        public Worker(MessageDigest md) {
            this.md = md;
        }

        public void run() {
            byte[] buffer = new byte[BUFFER_SIZE];
            while (work) {
                try {
                    FileHashJob job = jobs.take();
                    if (job != null) {
                        InputStream in = new FileInputStream(job.getFile());
                        int read;
                        while ((read = in.read(buffer, 0, BUFFER_SIZE)) != -1) {
                            md.update(buffer, 0, read);
                        }
                        String hash = new String(Hex.encode(md.digest()));
                        md.reset();
                        job.getEntry().setHash(hash);
                        if (jobsCounter.decrementAndGet() == 0) {
                            synchronized (monitor) {
                                monitor.notify();
                            }
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

}

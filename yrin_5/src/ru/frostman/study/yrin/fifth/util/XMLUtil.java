package ru.frostman.study.yrin.fifth.util;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;
import ru.frostman.study.yrin.fifth.model.BaseEntry;
import ru.frostman.study.yrin.fifth.model.DirEntry;
import ru.frostman.study.yrin.fifth.model.FileEntry;
import ru.frostman.study.yrin.fifth.model.StoredFile;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.HashMap;
import java.util.Map;

/**
 * @author slukjanov aka Frostman
 */
public class XMLUtil {
    public static void writeToXML(BaseEntry baseEntry, File file) {
        try {
            DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = builderFactory.newDocumentBuilder();

            Document document = builder.newDocument();
            Element files = document.createElement("files");
            document.appendChild(files);

            addElements(document, files, baseEntry, null);

            TransformerFactory transformerFactory = TransformerFactory.newInstance();

            Transformer transformer = transformerFactory.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.transform(new DOMSource(document),
                    new StreamResult(new FileOutputStream(file)));
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void addElements(Document document, Element root, BaseEntry entry, File filePath) {
        if (entry instanceof FileEntry && filePath != null) {
            Element file = document.createElement("file");
            Element path = document.createElement("path");
            Element hash = document.createElement("hash");

            path.appendChild(document.createTextNode(filePath.getAbsolutePath()));
            hash.appendChild(document.createTextNode(((FileEntry) entry).getHash()));
            file.appendChild(path);
            file.appendChild(hash);

            root.appendChild(file);
        } else if (entry instanceof DirEntry) {
            for (Map.Entry<File, BaseEntry> pair : ((DirEntry) entry).getEntries().entrySet()) {
                addElements(document, root, pair.getValue(), pair.getKey());
            }
        }
    }

    public static Map<File, StoredFile> readStoredFiles(File file) {
        try {
            SAXParserFactory parserFactory = SAXParserFactory.newInstance();
            SAXParser parser = parserFactory.newSAXParser();

            final Map<File, StoredFile> files = new HashMap<File, StoredFile>();

            if (!file.exists()) {
                return files;
            }

            parser.parse(new FileInputStream(file), new DefaultHandler() {
                private String data;
                private StoredFile file;

                @Override
                public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
                    if ("file".equals(qName)) {
                        file = new StoredFile();
                    }
                }

                @Override
                public void endElement(String uri, String localName, String qName) throws SAXException {
                    if ("path".equals(qName)) {
                        file.setFile(new File(data));
                    } else if ("hash".equals(qName)) {
                        file.setHash(data);
                    } else if ("file".equals(qName)) {
                        files.put(file.getFile(), file);
                        file = null;
                    }
                }

                @Override
                public void characters(char[] ch, int start, int length) throws SAXException {
                    data = new String(ch, start, length);
                }
            });

            return files;
        } catch (SAXParseException e) {
            return new HashMap<File, StoredFile>();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}

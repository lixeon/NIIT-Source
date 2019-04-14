/*
 * DVDLibrary.java
 *
 * 
 */

package com.dvd.model;

import java.io.*;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 *
 * @author 
 */
public class DVDLibrary implements DVDLibraryInterface{
    
    private File libraryFile;
    private List dvdCollection;
    private List genreList;
    
    public DVDLibrary(String dataDirectory) {
        // Initialize data directory and collection file
        this.libraryFile = new File(dataDirectory, "my-library.txt");
        // Initialize and load the DVD collection
        this.dvdCollection = new ArrayList();
        readFile();
        // Initialize and populate the genre list
        this.genreList = new ArrayList();
        this.genreList.add("Drama");
        this.genreList.add("Sci-Fi");
        this.genreList.add("Romance");
        this.genreList.add("Musical");
        this.genreList.add("Action");
        this.genreList.add("Comedy");
    }
    
    public List getDVDCollection() {
        return Collections.unmodifiableList(dvdCollection);
    }
    
    public DVDItem addDVD(String title, String year, String genre) {
        DVDItem item = new DVDItem(title, year, genre);
        dvdCollection.add(item);
        appendToFile(item);
        return item;
    }
    
    public List getGenres() {
        return Collections.unmodifiableList(genreList);
    }
    
    public void addGenre(String new_genre) {
        if ( ! genreList.contains(new_genre) ) {
            genreList.add(new_genre);
        }
    }
    
    private void readFile() {
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(libraryFile));
            String line;
            
            // Read every dvd entry in the file
            while ( (line = reader.readLine()) != null ) {
                String[] elements = line.split("\\|");
                
                // Extract the data fields for the record
                String title = elements[0];
                String year = elements[1];
                String genre = elements[2];
                
                // Add the new League item to the set
                DVDItem item = new DVDItem(title, year, genre);
                dvdCollection.add(item);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if ( reader != null ) {
                try { reader.close(); } catch (Exception e) {}
            }
        }
    }
    
    private void appendToFile(DVDItem item) {
        PrintWriter libraryWriter = null;
        try {
            // Open a writer stream and mark it to append the new data
            libraryWriter = new PrintWriter(new FileWriter(libraryFile, true));
            
            libraryWriter.print(item.getTitle());
            libraryWriter.print('|' + item.getYear());
            libraryWriter.print('|' + item.getGenre() );
            libraryWriter.println();
            
        } catch (Exception e) {
            throw new RuntimeException(e);
            
            // Clean up IO resources
        } finally {
            if ( libraryWriter != null ) {
                try { libraryWriter.close(); } catch (Exception e) { System.err.println(e); }
            }
        }
    }    
}

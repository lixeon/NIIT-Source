package com.dvd.model;

import java.util.List;
import java.util.ArrayList;
import java.io.*;

public class DVDLibrary implements DVDLibraryInterface {
    
    private String username = null;
    private List dvdCollection = null;
    private List genreList = null;
    private DVDLibraryDAO dao = null;
    
    public DVDLibrary(String username) {
        this.username = username;
        this.dao = new DVDLibraryDAO();
    }
    
    public List getDVDCollection() {
        if ( dvdCollection == null ) {
            dvdCollection = dao.getDVDLibrary(username);
        }
        return dvdCollection;
    }
    
    public DVDItem addDVD(String title, String year, String genre) {
        DVDItem item = new DVDItem(title, year, genre);
        // Store in the cache
        List dvds = getDVDCollection();
        dvds.add(item);
        // Store in the database
        dao.addDVD(username, item);
        // Return the item to the client
        return item;
    }
    
    public List getGenres() {
        if ( genreList == null ) {
            genreList = dao.getGenres(username);
        }
        return genreList;
    }
    
    public void addGenre(String new_genre) {
        if ( ! genreList.contains(new_genre) ) {
            genreList.add(new_genre);
        }
    }
    
}

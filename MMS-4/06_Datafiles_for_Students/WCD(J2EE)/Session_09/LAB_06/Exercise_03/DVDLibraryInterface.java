/*
 * DVDLibraryInterface.java
 *
 * 
 */

package com.dvd.model;

import java.util.List;

/**
 *
 * @author 
 */
public interface DVDLibraryInterface {    
    
    // Return a read-only reference the Collection of DVDs in the library
    public List getDVDCollection();
    
    // Given a title, a year and a genre, 
    // 1.) create a new DVD item, 
    // 2.) add the new item to the library
    // 3.) return a reference to the new DVD item
    public DVDItem addDVD(String title, String year, String genre);
    
    // Return a read-only reference 
    // a List of genres currently recognized by the library
    public List getGenres();
    
    // Given the name of a genre, 
    // check if that genre is present in the current genre list, 
    // if not present add the new genre to the list
    public void addGenre(String new_genre);
    
}



package com.dvd.model;

import com.dvd.model.DVDItem;

import java.sql.*;
import javax.sql.DataSource;

import javax.naming.Context;
import javax.naming.InitialContext;

import java.util.ArrayList;
import java.util.List;

public class DVDLibraryDAO {
    
    private static final String GET_LIBRARY_ITEMS = "SELECT id, title, \"year\", genre FROM Item WHERE username=?";
    private static final String CREATE_DVD_ITEM = "INSERT INTO Item (id, username, title, \"year\", genre) VALUES (?, ?, ?, ?, ?)";
    private static final String GET_GENRES = "SELECT DISTINCT genre FROM Item WHERE username=?";
    
    private static final String GET_NEXT_ID = "SELECT id FROM ObjectIDs WHERE table_name=?";
    private static final String UPDATE_ID = "UPDATE ObjectIDs SET id=? WHERE table_name=?";
    private static final String CREATE_ID = "INSERT INTO ObjectIDs (table_name, id) VALUES (?, ?)";
    
    private DataSource datasource;
    
    public DVDLibraryDAO() {
        try {
            Context ctx = new InitialContext();
            datasource = (DataSource) ctx.lookup("java:comp/env/jdbc/dvdLibraryDB");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public List getGenres(String username) {
        List genres = new ArrayList();
        Connection con = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            con = datasource.getConnection();
            ps = con.prepareStatement(GET_GENRES);
            ps.setString(1, username);
            rs = ps.executeQuery();
            while(rs.next()) {
                genres.add(rs.getString("genre"));
            }
        } catch (SQLException se) {
            se.printStackTrace();
        } finally {
            if(rs != null) {
                try { rs.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(ps != null) {
                try { ps.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(con != null) {
                try { con.close(); } catch (Exception e) { e.printStackTrace(); }
            }
        }
        return genres;
    }
    
    public List getDVDLibrary(String username) {
        List dvdLibrary = new ArrayList();
        Connection con = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            con = datasource.getConnection();
            ps = con.prepareStatement(GET_LIBRARY_ITEMS);
            ps.setString(1, username);
            rs = ps.executeQuery();
            while(rs.next()) {
                DVDItem item = new DVDItem(
                        rs.getString("title"),
                        rs.getString("year"),
                        rs.getString("genre"));
                dvdLibrary.add(item);
            }
        } catch (SQLException se) {
            se.printStackTrace();
        } finally {
            if(rs != null) {
                try { rs.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(ps != null) {
                try { ps.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(con != null) {
                try { con.close(); } catch (Exception e) { e.printStackTrace(); }
            }
        }
        return dvdLibrary;
    }
    
    public void addDVD(String username, DVDItem item) {
        Connection con = null;
        PreparedStatement ps = null;
        try {
            int id = getNextID("Item");
            con = datasource.getConnection();
            ps = con.prepareStatement(CREATE_DVD_ITEM);
            ps.setInt(1, id);
            ps.setString(2, username);
            ps.setString(3, item.getTitle());
            ps.setString(4, item.getYear());
            ps.setString(5, item.getGenre());
            ps.executeUpdate();
        } catch (SQLException se) {
            se.printStackTrace();
        } finally {
            if(ps != null) {
                try { ps.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(con != null) {
                try { con.close(); } catch (Exception e) { e.printStackTrace(); }
            }
        }
    }
    
    public int getNextID(String tableName) {
        int id = 1;
        Connection con = null;
        PreparedStatement queryStmt = null;
        PreparedStatement updateStmt = null;
        ResultSet rs = null;
        try {
            con = datasource.getConnection();
            queryStmt = con.prepareStatement(GET_NEXT_ID);
            queryStmt.setString(1, tableName);
            rs = queryStmt.executeQuery();
            if(rs.next()) {
                id = rs.getInt("id");
                updateStmt = con.prepareStatement(UPDATE_ID);
                updateStmt.setInt(1, id+1);
                updateStmt.setString(2, tableName);
                updateStmt.executeUpdate();
            } else {
                updateStmt = con.prepareStatement(CREATE_ID);
                updateStmt.setString(1, tableName);
                updateStmt.setInt(2, id+1);
                updateStmt.executeUpdate();
            }
        } catch (SQLException se) {
            se.printStackTrace();
        } finally {
            if(rs != null) {
                try { rs.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(queryStmt != null) {
                try { queryStmt.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(updateStmt != null) {
                try { updateStmt.close(); } catch (Exception e) { e.printStackTrace(); }
            }
            if(con != null) {
                try { con.close(); } catch (Exception e) { e.printStackTrace(); }
            }
        }
        
        return id;
    }
    
}

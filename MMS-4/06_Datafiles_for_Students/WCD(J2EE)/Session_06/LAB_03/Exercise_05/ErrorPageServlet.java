/*
 * ErrorPageServlet.java
 *
 * 
 */

package com.dvd.view;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// Support classes
import java.io.IOException;
import java.io.PrintWriter;
// Model classes
import java.util.List;
import java.util.Iterator;

/**
 *
 * @author 
 * @version
 */

public class ErrorPageServlet extends HttpServlet {
    
    public void doGet(HttpServletRequest request,
            HttpServletResponse response)
            throws IOException {
        processRequest(request, response);
    }
    
    public void doPost(HttpServletRequest request,
            HttpServletResponse response)
            throws IOException {
        processRequest(request, response);
    }
    
    public void processRequest(HttpServletRequest request,
            HttpServletResponse response)
            throws IOException {
        
        // Retrieve the errorMsgs from the request-scope
        List errorMsgs = (List) request.getAttribute("errorMsgs");
        
        // Specify the content type is HTML
        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        
        // Generate the HTML response
        out.println("<html>");
        out.println("<head>");
        out.println("<title>DVD Library Application: Error Page</title>");
        out.println("</head>");
        out.println("<body bgcolor='white'>");
        out.println("<h2>Error Report</h2>");
        out.println("<font color='red'>Please correct the following errors:");
        out.println("<ul>");
        Iterator items = errorMsgs.iterator();
        while ( items.hasNext() ) {
            String message = (String) items.next();
            out.println("  <li>" + message + "</li>");
        }
        out.println("</ul>");
        out.println("Back up and try again.");
        out.println("</font>");
        out.println("</body>");
        out.println("</html>");
        out.close();
    }
}

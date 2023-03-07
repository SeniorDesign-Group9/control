// web.hh

#ifndef WEBSERVER_H
#define WEBSERVER_H


// WiFi class
class WebServer {
    public:
        // Singleton functions
        // Get WebServer instance
        static WebServer& instance();

        // Disallow copying
        WebServer& operator = (const WebServer&) = delete;
        WebServer(const WebServer&) = delete;

        // Disallow moving
        WebServer& operator = (WebServer&&) = delete;
        WebServer(WebServer&&) = delete;

        // Class functions
        // Start HTTP server
        int start(void);

        // Stop HTTP server
        int stop(void);
    
    private:
        // Singleton variables
        WebServer();
        ~WebServer();

        // Class variables

};

#endif // WEBSERVER_H

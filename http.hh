// http.hh

#ifndef HTTPSERVER_H
#define HTTPSERVER_H


// WiFi class
class HttpServer {
    public:
        // Singleton functions
        // Get HttpServer instance
        static HttpServer& instance();

        // Disallow copying
        HttpServer& operator = (const HttpServer&) = delete;
        HttpServer(const HttpServer&) = delete;

        // Disallow moving
        HttpServer& operator = (HttpServer&&) = delete;
        HttpServer(HttpServer&&) = delete;

        // Class functions
        // Start HTTP server
        int start(void);

        // Stop HTTP server
        int stop(void);
    
    private:
        // Singleton variables
        HttpServer();
        ~HttpServer();

        // Class variables

};

#endif // HTTPSERVER_H

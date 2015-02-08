#ifndef FILESERVER_H
#define FILESERVER_H

#include <SPI.h>
#include <Ethernet.h>
#include <WebServer.h>


Sd2Card *card;
SdVolume volume;
SdFile root;

#define error(s) error_P(PSTR(s))

void error_P(const char* str) {
  PgmPrint("error: ");
  SerialPrintln_P(str);
  if (card->errorCode()) {
    PgmPrint("SD error: ");
    Serial.print(card->errorCode(), HEX);
    Serial.print(',');
    Serial.println(card->errorData(), HEX);
  }
  while(1);
}

void initFileServer(void)
{
	card = sd.card(); // if (!card.init(SPI_HALF_SPEED, 4)) error("card.init failed!");
	
	// initialize a FAT volume
	if (!volume.init(card)) error("vol.init failed!");

	PgmPrint("Volume is FAT");
	Serial.println(volume.fatType(),DEC);
	Serial.println();
	
	if (!root.openRoot(&volume)) error("openRoot failed");
}

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    P(helloMsg) = "<h1>Hello, World!</h1>";
    /* this is a special form of print that outputs from PROGMEM */
    server.printP(helloMsg);
	server.print("<li><a href=\"fs\">Filesystem</a></li>");
  }
}

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void restCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();
  
  if (type == WebServer::GET)
  {
    
    
    
  }

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    P(helloMsg) = "<h1>This is the Arduheat RestAPI.</h1>";
    /* this is a special form of print that outputs from PROGMEM */
    server.printP(helloMsg);
  }
}

// #define WEBDUINO_SERVER_HEADER

void httpNotFound(WebServer &server){
  P(failMsg) =
    "HTTP/1.0 404 Bad Request" CRLF 
    "Content-Type: text/html" CRLF 
    "Server: Webduino/" WEBDUINO_VERSION_STRING CRLF CRLF 
    "<h2>File Not Found !</h2>";
  server.printP(failMsg);
}


void fsAccessCmd(WebServer &server, WebServer::ConnectionType type, char **url_path, char *url_tail, bool tail_complete)
{
  /*
    Use the following to test 
      curl "192.168.1.80/fs"
      curl "192.168.1.80/fs/TESTFILE.INO"
      curl -X DELETE "192.168.1.80/fs/TESTFILE.INO"
    Sources
     - http://www.ladyada.net/learn/arduino/ethfiles.html
    Improvements
     - Expose a WebDav interface http://blog.coralbits.com/2011/07/webdav-protocol-for-dummies.html
   */
  if(!tail_complete) server.httpServerError();
  //Only serve files under the "/fs" path
  if(strncmp(url_path[0],"fs",3)!=0){
//     DEBUG_PRINT_PL("Path not found 404");
    httpNotFound(server);
    return;
  }
  if(url_path[1]==0){
    // do an ls
    server.httpSuccess();
    dir_t p;
    root.rewind();
    server.println("<ul>");
    while (root.readDir(&p) > 0) {
      // done if past last used entry
      if (p.name[0] == DIR_NAME_FREE) break;
      
      // skip deleted entry and entries for . and  ..
      if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;
      
      // print any indent spaces
      server.print("<li><a href=\"fs\\");
      for (uint8_t i = 0; i < 11; i++) {
        if (p.name[i] == ' ') continue;
        if (i == 8) {
          server.print('.');
        }
        server.print((char)p.name[i]);
      }
      
      server.print("\">");
      // print file2 name with possible blank fill
      for (uint8_t i = 0; i < 11; i++) {
	if (p.name[i] == ' ') continue;
	if (i == 8) {
	  server.print('.');
	}
	server.print((char)p.name[i]);
      }
      
      server.print("</a>");
	
      if (DIR_IS_SUBDIR(&p)) {
	server.print('/');
      }
      // print modify date/time if requested
//       if (flags & LS_DATE) {
// 	root.printFatDate(p.lastWriteDate);
// 	server.print(' ');
// 	root.printFatTime(p.lastWriteTime);
//       }
      // print size if requested
//       if (!DIR_IS_SUBDIR(&p) && (flags & LS_SIZE)) {
	server.print(' ');
	server.print(p.fileSize);
//       }
      
      server.println("</li>");
	
    }
    server.println("</ul>");
  }
  else{
    // access a file
    SdFile file;
    if (! file.open(&root, url_path[1], O_READ)) {
      httpNotFound(server);
    } 
    else {
      if(type==WebServer::GET){
        server.httpSuccess("text/plain");
	
	unsigned char c[WEBDUINO_OUTPUT_BUFFER_SIZE-2];
	int n=0;
	//PgmPrint("Free RAM: "); Serial.println(FreeRam());// part of sdFatUtil
	
        while ((n = file.read(c,WEBDUINO_OUTPUT_BUFFER_SIZE-2)) > 0) {
	  server.write(c,n);
        }
      }
      else if(type==WebServer::DELETE){
//         DEBUG_PRINT_PL("DELETE");
        server.httpSuccess();
//          card.remove(url_path[1]);
      }
      file.close();
    }
  }
}

#endif
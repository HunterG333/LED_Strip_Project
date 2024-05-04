const http = require("http");
const fs = require('fs').promises;
const { exec } = require('child_process');

const host = '192.168.4.46';
const port = 8000;

let indexFile;

const requestListener = function (req, res) {
  if (req.method === 'POST' && req.url === '/command') {
    let data = '';
    req.on('data', chunk => {
      data += chunk;
    });

    req.on('end', () => {
      try {
        const parsedData = JSON.parse(data);
        console.log('Received command:', parsedData.command);
        
        // Execute the command received from the client
        exec(parsedData.command, (error, stdout, stderr) => {
          if (error) {
            console.error(`Error executing command: ${error}`);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal server error' }));
            return;
          }
          console.log('Command output:', stdout);
          
          // Send response back to the client
          res.writeHead(200, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ message: 'Command executed successfully' }));
        });
      } catch (error) {
        console.error('Error parsing JSON:', error);
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Invalid JSON' }));
      }
    });
  } else if (req.method === 'GET') {
    res.setHeader("Content-Type", "text/html");
    res.writeHead(200);
    res.end(indexFile);
  }
};

const server = http.createServer(requestListener);
fs.readFile(__dirname + "/index.html")
  .then(contents => {
    indexFile = contents;
    server.listen(port, host, () => {
      console.log(`Server is running on http://${host}:${port}`);
    });
  })
  .catch(err => {
    console.error(`Could not read index.html file: ${err}`);
    process.exit(1);
  });

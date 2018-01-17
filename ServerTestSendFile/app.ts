import http = require('http');
import fs = require('fs');
import util = require('util');

declare module 'util' {
    export function promisify<T>(
        func: (data: any, cb: (err: NodeJS.ErrnoException, data?: T) => void,
        ) => void): (...input: any[]) => Promise<T>;
}

const server: http.Server = http.createServer();
server.on('request', (request: http.IncomingMessage, response: http.ServerResponse) => {
    const { headers, method, url } = request;
    request.setEncoding = null;

    let data: Array<any> = [];
    let body: string;

    request.on('error', (err: Error) => {
        console.error(err);
    }).on('data', (chunk) => {
        data.push(chunk);
    }).on('end', async () => {
        let binary = Buffer.concat(data);

        response.on('error', (err) => {
            console.error(err);
        });

        const fs_writeFile = util.promisify(fs.writeFile);
        await fs_writeFile("test.zip", binary);

        response.writeHead(200, {'Content-Type': 'text/plain' });
        response.end('Transfer complete\n');
    });
});
server.listen(8080);
import * as http from 'http';
import * as ApiSendReplays from './Api/SendReplays';
import * as ApiSendResult from './Api/SendResult';

import * as fs from 'fs';
import * as yauzl from 'yauzl';

const server: http.Server = http.createServer();
server.on('request', (request: http.IncomingMessage, response: http.ServerResponse) => {
    const { headers, method, url } = request;
   // request.setEncoding = null;

    let data: Array<any> = [];

    request.on('error', (err: Error) => {
        console.error(err);
    }).on('data', (chunk) => {
        data.push(chunk);
    }).on('end', async () => {
        response.on('error', (err) => {
            console.error(err);
        });
        
        let binary = Buffer.concat(data);
        let result: boolean;
        if (url === '/SendResult') {
            result = await ApiSendResult.Handle(binary);

        } else if (url === '/SendReplays') {
            let identity: string = <string>headers.identity;

            result = await ApiSendReplays.Handle(binary);

        }

        response.writeHead(200, {'Content-Type': 'text/plain' });
        response.end('Transfer complete\n');
    });
});
server.listen(8080);
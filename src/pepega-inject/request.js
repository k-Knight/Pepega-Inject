const https = require('https');
const http = require('http');

const request = function(url, options) {
    return new Promise((resolve, reject) => {
        const lib = url.startsWith('https') ? https : http;

        const request = lib.request(url, options, (response) => {
            if (response.statusCode < 200 || response.statusCode > 299)
                reject(new Error('Request failed, status code: ' + response.statusCode));

            let data = [];

            response.on('data', (chunk) => { data.push(chunk); });
            response.on('end', () => { resolve(Buffer.concat(data)); });
        });

        request.on('error', (err) => { reject(err); })
        request.on('timeout', () => { reject(new Error('Request timed out')); });
        request.end();
    });
}

const requestConcurrent = function(urls, options, callback) {
    let pendingCount = urls.length;
    let results = new Array(urls.length);
    let joiner = (id, result) => {
        results[id] = result;
        pendingCount--;

        if (pendingCount == 0)
            callback(results);
    };

    for (let id = 0; id < urls.length; id++)
        request(urls[id], options).then((data) => {
            joiner(id, data);
        }).catch((err) => {
            joiner(id, null);
        });
}

module.exports = {
    request,
    requestConcurrent
}
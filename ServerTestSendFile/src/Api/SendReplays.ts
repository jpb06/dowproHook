import * as util from 'util';
import * as fs from 'fs';
import * as yauzl from 'yauzl';

export async function Handle(data: Buffer): Promise<boolean> {
    const fs_writeFile = util.promisify(fs.writeFile);
    await fs_writeFile('test.zip', data);

    yauzl.open('test.zip', { lazyEntries: true }, (err, zipfile) => {

        if (err) throw err;

        zipfile.readEntry();

        zipfile.on('entry', (entry) => {
            if (/\/$/.test(entry.fileName)) {
                // Directory file names end with '/'.
                zipfile.readEntry();
            } else {
                // file entry
                zipfile.openReadStream(entry, function (err, readStream) {
                    if (err) throw err;
                    readStream.on("end", function () {
                        zipfile.readEntry();
                    });
                    var myFile = fs.createWriteStream(entry.fileName);
                    readStream.pipe(myFile);
                });
            }

            zipfile.readEntry();
        });
        zipfile.on('end', () => {
            console.log('end');
        });
    });

    return true;
}
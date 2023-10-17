const hdfswrapper = require('../build/Release/hdfswrapper.node');
import hdfswrapper from '../build/Release/hdfswrapper.node';

class HdfsClient {
    private hdfsFile: any;

    constructor(host: string, port: number, path: string) {
        this.hdfsFile = new hdfswrapper.HdfsFile(host, port, path);
    }

    read(): string {
        return this.hdfsFile.read();
    }

    close(): void {
        this.hdfsFile.close();
    }
}

export default HdfsClient;
import HdfsClient from '../src/hdfsClient';

const client = new HdfsClient('localhost', 9000, '/path/to/hdfs/file.txt');
try {
    const content = client.read();
    console.log(content);
} finally {
    client.close();
}
const fs = require('fs');
const stream = fs.createWriteStream(__dirname + "/../generated-maze.txt");

// Setup variables
const size = 10;
const start = { x: 0, y: 0 };
const end = { x: 9, y: 9 };
const wallProb = 0.2;
const separator = ' ';

const maze = [];
for (let i = 0; i < size; i += 1) {
    const array = [];
    for (let j = 0; j < size; j += 1) {
        const rand = Math.random();
        if (rand < wallProb) array.push(1);
        else array.push(0);
    }
    maze.push(array);
}

// Making sure the start and end are empty
maze[start.y][start.x] = 0;
maze[end.y][end.x] = 0;

stream.once('open', function(fd) {
    stream.write(`${size} ${size}\n`);
    stream.write(`${start.x} ${start.y}\n`);
    stream.write(`${end.x} ${end.y}\n`);
    maze.forEach((line) => {
        const string = line.join(separator) + "\n";
        stream.write(string);
    });
    stream.end();
});

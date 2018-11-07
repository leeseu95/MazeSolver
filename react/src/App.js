import React from 'react';

const mazeFile = './maze5.txt';
const solutionFile = './solution5.txt';
const tick = 50;

export default class App extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            ready: false,
            counter: 2,
            maze: [],
            ansMaze: [],
            solutionLength: 0,
            mazeSize: { x: 0, y: 0 },
            start: { x: 0, y: 0 },
            end: { x: 0, y: 0 },
            size: 0,
        };
    }

    componentWillMount() {
        this.readMaze(mazeFile);
    }

    readMaze = (file) => {
        var rawFile = new XMLHttpRequest();
        rawFile.open("GET", file, false);
        rawFile.onreadystatechange = () => {
            if (rawFile.readyState === 4) {
                if (rawFile.status === 200 || rawFile.status === 0) {
                    const maze = [];
                    rawFile.responseText.split("\n").forEach((line, index) => {
                        let [x, y] = line.split(" ");
                        x = parseInt(x, 10);
                        y = parseInt(y, 10);
                        if (index === 0) {
                            const _x = window.innerWidth / x;
                            const _y = window.innerHeight / y;
                            let size = _x;
                            if (_y < _x) size = _y;
                            this.setState({ mazeSize: { x, y }, size });
                        }
                        else if (index === 1) this.setState({ start: { x, y }});
                        else if (index === 2) this.setState({ end: { x, y }});
                        else if (line) maze.push(line.split('').map(string => parseInt(string, 10)));
                    });
                    this.setState({ maze }, () => {
                        this.readSolution(solutionFile);
                    });
                }
            }
        };
        rawFile.send(null);
    };

    readSolution = (file) => {
        var rawFile = new XMLHttpRequest();
        rawFile.open("GET", file, false);
        rawFile.onreadystatechange = () => {
            if (rawFile.readyState === 4) {
                if (rawFile.status === 200 || rawFile.status === 0) {
                    const solution = [];
                    let { x, y } = this.state.start;
                    solution.push([x, y]);
                    rawFile.responseText.split("\n").forEach((line, index) => {
                        if (index === 0) line.split("").forEach((step) => {
                            if (step === 'U') { y -= 1; solution.push([x, y]); }
                            else if (step === 'D') { y += 1; solution.push([x, y]); }
                            else if (step === 'L') { x -= 1; solution.push([x, y]); }
                            else { x += 1; solution.push([x, y]); }
                        });
                    });
                    this.setState({ solutionLength: solution.length }, () => this.applySolution(solution));
                }
            }
        };
        rawFile.send(null);
    };

    applySolution = (solution) => {
        const ansMaze = [];
        const { x, y } = this.state.mazeSize;
        // Hacemos una matriz del mismo tamaño que el maze y la llenamos de 0's
        for (let i = 0; i < y; i += 1) {
            const temp = [];
            for (let j = 0; j < x; j += 1) temp.push(0);
            ansMaze.push(temp);
        }

        // Metemos todos los pasos de la solución
        // Empezamos en 2 porque end es 1 y start está incluido en solution
        let counter = 2;
        solution.forEach((step) => {
            const [ _x, _y ] = step;
            ansMaze[_y][_x] = -counter;
            counter += 1;
        });

        const { end } = this.state;
        // Metemos el punto de end
        ansMaze[end.y][end.x] = 1;

        this.setState({ ansMaze }, () => this.getSolutionsNeighbours(solution));
    }

    getSolutionsNeighbours = (solution) => {
        let counter = 2;
        solution.forEach((step) => {
            const [x, y] = step;
            const { mazeSize } = this.state;
            const checkX = [0];
            const checkY = [0];
            if (x > 0) checkX.push(-1);
            if (x < mazeSize.x - 1) checkX.push(1);
            if (y > 0) checkY.push(-1);
            if (y < mazeSize.y - 1) checkY.push(1);
            checkX.sort();
            checkY.sort();
            const ansMaze = [...this.state.ansMaze];
            checkY.forEach((_y) => {
                checkX.forEach((_x) => {
                    if (ansMaze[y + _y][x + _x] === 0) {
                        ansMaze[y + _y][x +_x] = counter;
                    }
                });
            });
            counter += 1;
        });
        this.setState({ ready: true }, () => {
            this.startInterval();
        });
    }

    startInterval = () => {
        console.log('interval');
        this.interval = setInterval(() => {
            let { counter } = this.state;
            counter += 1;
            this.setState({ counter });
            if (counter === this.state.solutionLength + 1) this.cancelInterval();
        }, tick);
    }

    cancelInterval = () => {
        clearInterval(this.interval);
    }

    renderMaze() {
        const { counter, ansMaze, size, maze } = this.state;
        console.log('counter:', counter);
        return ansMaze.map((row, y) => {
            return (
                <div style={{ display: 'flex', flexDirection: 'row' }}>
                    {row.map((square, x) => {
                        // Donde termina lo pintamos desde el principio morado
                        if (square === 1) return <div style={{ height: size, width: size, backgroundColor: 'purple' }} />;
                        // En el que vamos, lo pintamos siempre en rojo
                        else if (square === -counter) return <div style={{ height: size, width: size, backgroundColor: 'red' }} />;
                        else if (square > 0 && square <= counter) {
                            if (maze[y][x] === 1) return <div style={{ height: size, width: size, backgroundColor: 'brown' }} />;
                            return <div style={{ height: size, width: size, backgroundColor: 'yellow' }} />;
                        }
                        else if (square < 0 && square > -counter) return <div style={{ height: size, width: size, backgroundColor: 'blue' }} />;
                        else if (square === -counter - 1) {
                            if (maze[y][x] === 1) return <div style={{ height: size, width: size, backgroundColor: 'brown' }} />;
                            return <div style={{ height: size, width: size, backgroundColor: 'yellow' }} />;
                        }
                        return <div style={{ height: size, width: size, backgroundColor: 'white' }} />;
                    })}
                </div>
            );
        });
    }

    render() {
        console.log(this.state);
        return (
            <div>
                {this.state.ready ? this.renderMaze() : null}
            </div>
        );
    }
}

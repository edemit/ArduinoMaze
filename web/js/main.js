let serialConnection = null;
let isSerialConnected = false;
let pollInterval = null;
let debugMode = false;
let serialBypass = false;
let commandPos = [0, 0];
let rotateInProgress = false;
let rotateTimeout = null;

const ROTATE_TIMEOUT_MS = 3000; // 3 secondes max pour une réponse


// Default port for ESP32-C3 via USB
const DEFAULT_PORT = '/dev/ttyUSB0';

// Check if we can connect to serial proxy server
function checkSerialProxySupport() {
    return 'fetch' in window;
}

// Connect to serial device via proxy server
async function startWebSerialConnect() {
    if (!checkSerialProxySupport()) {
        alert('Your browser does not support the required APIs.');
        return;
    }

    try {
        const baudRate = parseInt(document.getElementById('baudRate').value);
        // Read port from input if present, otherwise use default
        const portInput = document.getElementById('portPath');
        const port = (portInput && portInput.value.trim()) ? portInput.value.trim() : DEFAULT_PORT;

        logSerialOutput('📡 Attempting to connect to Arduino on ' + port + '...');

        const response = await fetch('http://localhost:3000/api/serial/connect', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                baudRate: baudRate,
                port: port           // FIX: tell the proxy exactly which port to open
            })
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const data = await response.json();

        if (data.success) {
            serialConnection = data.connectionId;
            isSerialConnected = true;

            updateConnectionStatus(true, data.port || port);
            logSerialOutput('✓ Connected to serial device at ' + baudRate + ' baud');
            logSerialOutput('→ Port: ' + (data.port || port));

            startSerialPolling();

            const testResponse = await fetch('http://localhost:3000/api/serial/send', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    connectionId: serialConnection,
                    message: "200\n"
                })
            });

            if (!testResponse.ok) {
                logSerialOutput('⚠ Warning: test message failed to send');
            }
        } else {
            throw new Error(data.message || 'Failed to connect');
        }

    } catch (error) {
        updateConnectionStatus(false);
        logSerialOutput('✗ Connection error: ' + error.message);
        console.error('Serial connection error:', error);
    }
}

// Disconnect from serial device
async function startWebSerialDisconnect() {
    clearRotateInProgress();
    try {
        if (pollInterval) {
            clearInterval(pollInterval);
            pollInterval = null;
        }

        if (serialConnection) {
            const response = await fetch('http://localhost:3000/api/serial/disconnect', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    connectionId: serialConnection
                })
            });

            if (response.ok) {
                serialConnection = null;
                isSerialConnected = false;
                updateConnectionStatus(false);   // FIX: sync the UI on disconnect
                logSerialOutput('✓ Disconnected from serial device');
            } else {
                throw new Error(await response.text());
            }
        }
    } catch (error) {
        logSerialOutput('✗ Disconnection error: ' + error.message);
        console.error('Serial disconnection error:', error);
    }
}

// Poll for data from serial device
async function startSerialPolling() {
    let lastId = 0;

    pollInterval = setInterval(async () => {
        if (!isSerialConnected || !serialConnection) return;

        try {
            const response = await fetch(
                `http://localhost:3000/api/serial/read/${serialConnection}?after=${lastId}&last=50`
            );

            if (!response.ok) return;

            const result = await response.json();

            if (result.success && result.data) {
                for (let i = 0; i < result.data.length; i++) {
                    const message = result.data[i].message.trim();
                    if (!message) continue;

                    logSerialOutput('📥 ' + message);

                    if (!mazeBuilded) {
                        const byteValue = parseInt(message, 10);

                        if (!isNaN(byteValue) && byteValue >= 0 && byteValue <= 255) {
                            mazeDataArray.push(byteValue);

                            if (mazeDataArray.length === 32) {
                                try {
                                    const mazeData = buildMazeFromData(mazeDataArray);
                                    renderMazeWithBorders(mazeData);
                                    mazeBuilded = true;
                                    logSerialOutput('✓ Maze built from serial data');
                                } catch (error) {
                                    logSerialOutput('✗ Error building maze: ' + error.message);
                                }
                                mazeDataArray = [];
                            }
                        }
                    } else {
                        const lines = message.split('\n');
                        lines.forEach(line => {
                            line = line.trim();
                            if (!line) return;

                            logSerialOutput("command input: " + line);

                            const mArray = line.split("/");

                            switch (mArray[0]) {
                                case "move":
                                    movePlayerByCommand(mArray[1]);
                                    break;

                                case "rotate":
                                    const targetRow = commandPos[0];
                                    const targetCol = commandPos[1];
                                    clearRotateInProgress();
                                    if (mArray[1] === "200") {
                                        const rotated = rotateCell(targetRow, targetCol);
                                        if (rotated) {
                                            logSerialOutput(`↻ Rotated cell at ${targetRow},${targetCol}`);
                                        }
                                    } else {
                                        logSerialOutput(`✗ Rotate failed for ${targetRow},${targetCol}`);
                                    }
                                    break;
                                case "fin":
                                    finishMaze();
                                    break;
                                default:
                                    logSerialOutput(`⚠ Unknown command: ${line}`);
                            }
                        });
                    }
                }

                if (result.latestId) {
                    lastId = result.latestId;
                }
            }

            if (!result.isConnected) {
                isSerialConnected = false;
                clearInterval(pollInterval);
                updateConnectionStatus(false);
                logSerialOutput('✗ Connection lost');
            }

        } catch (error) {
            console.error('Serial read error:', error);
            isSerialConnected = false;
            clearInterval(pollInterval);
            updateConnectionStatus(false);
            logSerialOutput('✗ Polling error: ' + error.message);
        }
    }, 500);
}

function setRotateInProgress(row, col) {
    rotateInProgress = true;
    commandPos = [row, col];

    // Timeout de sécurité : si pas de réponse, on débloque
    if (rotateTimeout) clearTimeout(rotateTimeout);
    rotateTimeout = setTimeout(() => {
        if (rotateInProgress) {
            logSerialOutput(`⚠ Rotate timeout for ${commandPos[0]},${commandPos[1]} — unlocking`);
            rotateInProgress = false;
            rotateTimeout = null;
        }
    }, ROTATE_TIMEOUT_MS);
}

function clearRotateInProgress() {
    rotateInProgress = false;
    if (rotateTimeout) {
        clearTimeout(rotateTimeout);
        rotateTimeout = null;
    }
}

// Send message to serial device
async function sendSerialMessage() {
    if (!isSerialConnected || !serialConnection) {
        logSerialOutput('✗ Error: Serial device not connected');
        return;
    }

    try {
        const message = document.getElementById('serialMessage').value.trim();
        if (!message) {
            logSerialOutput('✗ Error: No message to send');
            return;
        }

        const response = await fetch('http://localhost:3000/api/serial/send', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                connectionId: serialConnection,
                message: message
            })
        });

        if (response.ok) {
            logSerialOutput('📤 Sent: ' + message);
            document.getElementById('serialMessage').value = '';
        } else {
            const error = await response.json();
            throw new Error(error.message || 'Failed to send message');
        }
    } catch (error) {
        logSerialOutput('✗ Send error: ' + error.message);
        console.error('Serial send error:', error);
    }
}

// Log messages to serial output display
function logSerialOutput(message) {
    const outputDiv = document.getElementById('serial-output');
    const timestamp = new Date().toLocaleTimeString();
    const logEntry = document.createElement('div');
    logEntry.textContent = '[' + timestamp + '] ' + message;
    logEntry.style.padding = '5px';
    logEntry.style.borderBottom = '1px solid #ddd';
    outputDiv.appendChild(logEntry);
    outputDiv.scrollTop = outputDiv.scrollHeight;
}

// Clear serial output log
function clearSerialOutput() {
    const outputDiv = document.getElementById('serial-output');
    outputDiv.innerHTML = '';
    logSerialOutput('Log cleared');
}

// Update connection status display
function updateConnectionStatus(isConnected, portInfo = '') {
    const statusIndicator = document.getElementById('status-indicator');
    const statusText = document.getElementById('status-text');
    const connectionStatus = document.getElementById('connection-status');

    if (isConnected) {
        statusIndicator.textContent = '●';
        statusIndicator.style.color = '#4CAF50';
        statusText.textContent = '✓ Connected to ' + portInfo;
        connectionStatus.style.background = '#e8f5e9';
        connectionStatus.style.borderLeftColor = '#4CAF50';
        stopIdleAnimation();
    } else {
        statusIndicator.textContent = '●';
        statusIndicator.style.color = '#f44336';
        statusText.textContent = '✗ Not Connected';
        connectionStatus.style.background = '#ffebee';
        connectionStatus.style.borderLeftColor = '#f44336';
        startIdleAnimation();
    }
}

window.addEventListener('beforeunload', () => {
    if (isSerialConnected) {
        startWebSerialDisconnect();
    }
});

function movePlayerByCommand(direction) {
    let newRow = playerPos.row;
    let newCol = playerPos.col;

    switch (direction) {
        case 'up':
            newRow = Math.max(0, playerPos.row - 1);
            break;
        case 'down':
            newRow = Math.min(7, playerPos.row + 1);
            break;
        case 'left':
            newCol = Math.max(0, playerPos.col - 1);
            break;
        case 'right':
            newCol = Math.min(7, playerPos.col + 1);
            break;
        default:
            return false;
    }

    if (newRow === playerPos.row && newCol === playerPos.col) {
        return false;
    }

    if (!canMoveToCell(newRow, newCol)) {
        if (typeof logSerialOutput === 'function') {
            logSerialOutput(`✗ Move blocked by wall at ${playerPos.row},${playerPos.col}`);
        }
        return false;
    }

    movePlayerTo(newRow, newCol);
    return true;
}

function moveSelectedByCommand(direction) {
    if (!selectedPos) {
        selectedPos = { row: 0, col: 0 };
    }

    let newRow = selectedPos.row;
    let newCol = selectedPos.col;

    switch (direction) {
        case 'up':
            newRow = Math.max(0, selectedPos.row - 1);
            break;
        case 'down':
            newRow = Math.min(7, selectedPos.row + 1);
            break;
        case 'left':
            newCol = Math.max(0, selectedPos.col - 1);
            break;
        case 'right':
            newCol = Math.min(7, selectedPos.col + 1);
            break;
        default:
            return false;
    }

    if (newRow === selectedPos.row && newCol === selectedPos.col) {
        return false;
    }

    setSelectedCell(newRow, newCol);
    return true;
}

function movePlayerUp() {
    movePlayerByCommand('up');
}

function movePlayerDown() {
    movePlayerByCommand('down');
}

function movePlayerLeft() {
    movePlayerByCommand('left');
}

function movePlayerRight() {
    movePlayerByCommand('right');
}

async function interactionWithMatrix(command) {
    if (['up', 'down', 'left', 'right'].includes(command)) {
        moveSelectedByCommand(command);
        return;
    }
    if (command === 'rotate') {
        if (rotateInProgress) {
            logSerialOutput('⚠ Rotate already in progress, please wait...');
            return;
        }else{
            const targetRow = (typeof selectedPos !== 'undefined' && selectedPos) ? selectedPos.row : playerPos.row;
            const targetCol = (typeof selectedPos !== 'undefined' && selectedPos) ? selectedPos.col : playerPos.col;
            setRotateInProgress(targetRow, targetCol); // ← remplace l'assignation directe
            command = 'rotate/'+((targetCol)+(targetRow*8)); // Send rotate command to Arduino
        }
    }

    if (!isSerialConnected || !serialConnection) {
        logSerialOutput('✗ Error: Serial device not connected');
        return;
    }

    try {
        const response = await fetch('http://localhost:3000/api/serial/send', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                connectionId: serialConnection,
                message: command + '\n'
            })
        });

        if (response.ok) {
            logSerialOutput('📤 Sent: ' + command);
        } else {
            const error = await response.json();
            throw new Error(error.message || 'Failed to send message');
        }
    } catch (error) {
        logSerialOutput('✗ Send error: ' + error.message);
        console.error('Serial send error:', error);
    }
}
function toggleDebugMode() {
    debugMode = !debugMode;
    const debugPanel = document.getElementById('debug-panel');
    debugPanel.style.display = debugMode ? 'block' : 'none';
}
function parseMazeInput(input) {
    try {
        let dataArray;

        // Try JSON array format first
        if (input.trim().startsWith('[')) {
            dataArray = JSON.parse(input.trim());
        } else {
            // Try comma-separated format
            dataArray = input.split(',').map(val => {
                const num = parseInt(val.trim(), 10);
                if (isNaN(num)) throw new Error('Invalid number');
                return num;
            });
        }

        if (!Array.isArray(dataArray)) throw new Error('Input must be an array');
        if (dataArray.length !== 32) throw new Error('Array must contain exactly 32 integers');

        // Validate all values are 0-255
        for (let val of dataArray) {
            if (val < 0 || val > 255) throw new Error('All values must be between 0 and 255');
        }

        return dataArray;
    } catch (error) {
        alert(`❌ Invalid input: ${error.message}`);
        return null;
    }
}
function loadMazeFromDebug() {
    const input = document.getElementById('debug-maze-input').value;
    const dataArray = parseMazeInput(input);

    if (!dataArray) return;

    try {
        const mazeData = buildMazeFromData(dataArray);
        renderMazeWithBorders(mazeData);
        logSerialOutput('✓ Maze loaded from debug data');
    } catch (error) {
        alert(`❌ Error building maze: ${error.message}`);
        logSerialOutput(`✗ Maze load error: ${error.message}`);
    }
}
function generateRandomMaze() {
    const randomData = Array.from({ length: 32 }, () =>
        Math.floor(Math.random() * 256)
    );

    try {
        const mazeData = buildMazeFromData(randomData);
        renderMazeWithBorders(mazeData);

        // Display generated data
        document.getElementById('debug-maze-input').value = randomData.join(', ');
        logSerialOutput('✓ Random maze generated: [' + randomData.join(', ') + ']');
    } catch (error) {
        alert(`❌ Error generating maze: ${error.message}`);
        logSerialOutput(`✗ Random maze error: ${error.message}`);
    }
}

function toggleSerialBypass() {
    serialBypass = document.getElementById('bypass-serial').checked;
    if (serialBypass) {
        logSerialOutput('✓ Serial bypass enabled - local mode active');
    } else {
        logSerialOutput('✗ Serial bypass disabled - requires Arduino connection');
    }
}

function finishMaze() {
    showVictoryPopup();
}

function createConfetti() {
    const container = document.getElementById('confetti-container');
    if (!container) return;
    
    const confettiCount = 50;
    for (let i = 0; i < confettiCount; i++) {
        const confetti = document.createElement('div');
        confetti.className = 'confetti';
        
        const randomX = Math.random() * 100;
        const randomDelay = Math.random() * 0.3;
        const randomDuration = 2.5 + Math.random() * 1.5;
        
        confetti.style.left = randomX + '%';
        confetti.style.top = '-20px';
        confetti.style.width = (5 + Math.random() * 10) + 'px';
        confetti.style.height = confetti.style.width;
        confetti.style.borderRadius = Math.random() > 0.5 ? '0' : '50%';
        confetti.style.animationDelay = randomDelay + 's';
        confetti.style.animationDuration = randomDuration + 's';
        
        container.appendChild(confetti);
    }
}

function showVictoryPopup() {
    const victoryPopup = document.getElementById('victory-popup');
    if (victoryPopup) {
        victoryPopup.style.display = 'flex';
        createConfetti();
        logSerialOutput('🎉 Victoire! Vous avez gagné!');
    }
}

function closeVictoryPopup() {
    const victoryPopup = document.getElementById('victory-popup');
    if (victoryPopup) {
        victoryPopup.style.display = 'none';
        // Clear confetti
        const container = document.getElementById('confetti-container');
        if (container) {
            container.innerHTML = '';
        }
    }
}

function initDebugMode() {
    // Keyboard shortcut: Ctrl+Shift+D to show debug button
    document.addEventListener('keydown', (event) => {
        if (event.ctrlKey && event.shiftKey && event.code === 'KeyD') {
            event.preventDefault();
            const debugBtn = document.getElementById('debug-toggle-btn');
            debugBtn.style.display = debugBtn.style.display === 'none' ? 'block' : 'none';
        }
    });
    
    document.addEventListener('click', (event) => {
        if (event.target.classList.contains('maze_cell') && debugMode) {
            const row = parseInt(event.target.dataset.row);
            const col = parseInt(event.target.dataset.col);

            if (mazeWallConfig.length > 0 && mazeWallConfig[row] && mazeWallConfig[row][col]) {
                const walls = mazeWallConfig[row][col];
                const info = `Cell [${row}, ${col}]\n` +
                    `Right: ${walls.right ? '✓ Open' : '✗ Wall'}\n` +
                    `Up:    ${walls.up ? '✓ Open' : '✗ Wall'}\n` +
                    `Left:  ${walls.left ? '✓ Open' : '✗ Wall'}\n` +
                    `Down:  ${walls.down ? '✓ Open' : '✗ Wall'}`;
                document.getElementById('debug-cell-info').textContent = info;
            }
        }
    });
}

// Initialize debug mode when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    initDebugMode();
});
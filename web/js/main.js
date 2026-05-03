let serialConnection = null;
let isSerialConnected = false;
let pollInterval = null;
let debugMode = false;
let serialBypass = false;

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
    let lastDataLength = 0;
    pollInterval = setInterval(async () => {
        if (!isSerialConnected || !serialConnection) return;

        try {
            const response = await fetch(
                `http://localhost:3000/api/serial/read/${serialConnection}?last=50`
            );

            if (response.ok) {
                const result = await response.json();
                if (result.success && result.data && result.data.length > lastDataLength) {
                    for (let i = lastDataLength; i < result.data.length; i++) {
                        logSerialOutput('📥 ' + result.data[i].message);
                    }
                    lastDataLength = result.data.length;
                }

                if (!result.isConnected) {
                    isSerialConnected = false;
                    clearInterval(pollInterval);
                    updateConnectionStatus(false);
                    logSerialOutput('✗ Connection lost');
                }
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

async function interactionWithMatrix(command) {
    if (['up', 'down', 'left', 'right'].includes(command)) {
        movePlayerDirection(command);
        return;
    }
    if (command === 'rotate') {
        rotateCell(playerPos.row, playerPos.col);
        logSerialOutput('↻ Rotated cell at ' + playerPos.row + ',' + playerPos.col);
        return;
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
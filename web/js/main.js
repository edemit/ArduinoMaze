let serialConnection = null;
let isSerialConnected = false;
let pollInterval = null;

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

            // Start polling for data
            startSerialPolling();

            // FIX: test message is now inside the success block, after serialConnection is set
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
            // FIX: removed Content-Type header — GET requests have no body
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

// FIX: clean up server-side connection if tab is closed mid-session
// prevents "port busy" errors on the next connect attempt
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
let serialConnection = null;
let isSerialConnected = false;
let pollInterval = null;

// Check if we can connect to serial proxy server
function checkSerialProxySupport() {
    // Works with any browser that supports Fetch API
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
        
        logSerialOutput('📡 Attempting to connect to Arduino...');
        
        const response = await fetch('http://localhost:3000/api/serial/connect', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                baudRate: baudRate
            })
        });

        // After connection, send a test message to verify communication
        const connected = await fetch('http://localhost:3000/api/serial/send', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                connectionId: serialConnection,
                message: "200\n"
            })
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const data = await response.json();
        
        if (data.success) {
            serialConnection = data.connectionId;
            isSerialConnected = true;
            
            updateConnectionStatus(true, data.port || 'Unknown Port');
            logSerialOutput('✓ Connected to serial device at ' + baudRate + ' baud');
            logSerialOutput('→ Port: ' + (data.port || 'Auto-detected'));
            
            // Start polling for data
            startSerialPolling();
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
                const data = await response.json();
                serialConnection = null;
                isSerialConnected = false;
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
            const response = await fetch(`http://localhost:3000/api/serial/read/${serialConnection}?last=10`, {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json'
                }
            });

            if (response.ok) {
                const result = await response.json();
                if (result.success && result.data && result.data.length > lastDataLength) {
                    // Only log new messages
                    for (let i = lastDataLength; i < result.data.length; i++) {
                        logSerialOutput('📥 ' + result.data[i].message);
                    }
                    lastDataLength = result.data.length;
                }
                
                // Check if connection is still active
                if (!result.isConnected) {
                    isSerialConnected = false;
                    clearInterval(pollInterval);
                    logSerialOutput('✗ Connection lost');
                }
            }
        } catch (error) {
            console.error('Serial read error:', error);
            isSerialConnected = false;
            clearInterval(pollInterval);
            logSerialOutput('✗ Polling error: ' + error.message);
        }
    }, 500); // Poll every 500ms
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
            const data = await response.json();
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
    } else {
        statusIndicator.textContent = '●';
        statusIndicator.style.color = '#f44336';
        statusText.textContent = '✗ Not Connected';
        connectionStatus.style.background = '#ffebee';
        connectionStatus.style.borderLeftColor = '#f44336';
    }
}


async function interactionWithMatrix(command) {
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
                message: command + '\n' // Ensure command is sent with newline if needed
            })
        });

        if (response.ok) {
            const data = await response.json();
            //logSerialOutput(command);
        } else {
            const error = await response.json();
            throw new Error(error.message || 'Failed to send message');
        }
    } catch (error) {
        logSerialOutput('✗ Send error: ' + error.message);
        console.error('Serial send error:', error);
    }
}
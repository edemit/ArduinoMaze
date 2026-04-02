/**
 * ArduinoMaze Serial Proxy Server
 * Node.js Express server for web-based serial communication with Arduino Mega
 */

const express = require('express');
const cors = require('cors');
const path = require('path');
const { SerialPort, ReadlineParser } = require('serialport');

const app = express();
const PORT = 3000;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, 'web')));

// Serial connections storage
let serialConnections = {};
let connectionCounter = 0;

// Helper function to list available serial ports
async function listSerialPorts() {
    try {
        const ports = await SerialPort.list();
        return ports;
    } catch (error) {
        console.error('Error listing serial ports:', error);
        return [];
    }
}

// API: Get available serial ports
app.get('/api/serial/ports', async (req, res) => {
    try {
        const ports = await listSerialPorts();
        const portInfo = ports.map(port => ({
            path: port.path,
            manufacturer: port.manufacturer,
            serialNumber: port.serialNumber,
            pnpId: port.pnpId
        }));
        res.json({ success: true, ports: portInfo });
    } catch (error) {
        res.status(500).json({ success: false, message: error.message });
    }
});

// API: Connect to serial port
app.post('/api/serial/connect', async (req, res) => {
    try {
        const { baudRate = 115200, port = null } = req.body;
        
        // Get available ports
        const ports = await SerialPort.list();
        let selectedPort = port;
        
        if (!selectedPort && ports.length > 0) {
            // Auto-select first available port (usually Arduino)
            selectedPort = ports[0].path;
        }
        
        if (!selectedPort) {
            return res.status(400).json({ 
                success: false, 
                message: 'No serial port found. Please connect your Arduino.' 
            });
        }

        // Create serial port connection
        const serialPort = new SerialPort({
            path: selectedPort,
            baudRate: parseInt(baudRate),
            autoOpen: false
        });

        const parser = new ReadlineParser({ delimiter: '\n' });
        serialPort.pipe(parser);

        serialPort.open((err) => {
            if (err) {
                return res.status(400).json({ 
                    success: false, 
                    message: `Failed to open port: ${err.message}` 
                });
            }

            const connectionId = ++connectionCounter;
            serialConnections[connectionId] = {
                port: serialPort,
                parser: parser,
                data: [],
                isConnected: true
            };

            console.log(`Serial connection established on ${selectedPort} at ${baudRate} baud`);

            // Handle incoming data
            parser.on('data', (line) => {
                console.log(`[Arduino]: ${line}`);
                serialConnections[connectionId].data.push({
                    timestamp: Date.now(),
                    message: line
                });
                // Keep only last 100 messages
                if (serialConnections[connectionId].data.length > 100) {
                    serialConnections[connectionId].data.shift();
                }
            });

            serialPort.on('error', (error) => {
                console.error('Serial port error:', error);
                serialConnections[connectionId].isConnected = false;
            });

            serialPort.on('close', () => {
                console.log('Serial port closed');
                serialConnections[connectionId].isConnected = false;
            });

            res.json({ 
                success: true, 
                connectionId: connectionId,
                port: selectedPort,
                baudRate: baudRate,
                message: `Connected to ${selectedPort}`
            });
        });

    } catch (error) {
        console.error('Connection error:', error);
        res.status(500).json({ success: false, message: error.message });
    }
});

// API: Disconnect from serial port
app.post('/api/serial/disconnect', (req, res) => {
    try {
        const { connectionId } = req.body;
        
        if (!connectionId || !serialConnections[connectionId]) {
            return res.status(400).json({ 
                success: false, 
                message: 'Invalid connection ID' 
            });
        }

        const conn = serialConnections[connectionId];
        if (conn.isConnected) {
            conn.port.close((err) => {
                if (err) {
                    return res.status(400).json({ 
                        success: false, 
                        message: err.message 
                    });
                }
                conn.isConnected = false;
                delete serialConnections[connectionId];
                console.log('Serial connection closed');
                res.json({ success: true, message: 'Disconnected' });
            });
        } else {
            delete serialConnections[connectionId];
            res.json({ success: true, message: 'Already disconnected' });
        }
    } catch (error) {
        res.status(500).json({ success: false, message: error.message });
    }
});

// API: Send data to serial port
app.post('/api/serial/send', (req, res) => {
    try {
        const { connectionId, message } = req.body;
        
        if (!connectionId || !serialConnections[connectionId]) {
            return res.status(400).json({ 
                success: false, 
                message: 'Invalid connection ID' 
            });
        }

        if (!message) {
            return res.status(400).json({ 
                success: false, 
                message: 'Message is required' 
            });
        }

        const conn = serialConnections[connectionId];
        if (!conn.isConnected) {
            return res.status(400).json({ 
                success: false, 
                message: 'Serial port not connected' 
            });
        }

        // Send message with newline
        conn.port.write(`${message}\n`, (err) => {
            if (err) {
                return res.status(400).json({ 
                    success: false, 
                    message: `Send error: ${err.message}` 
                });
            }
            console.log(`[Sent]: ${message}`);
            res.json({ success: true, message: 'Message sent' });
        });
    } catch (error) {
        res.status(500).json({ success: false, message: error.message });
    }
});

// API: Receive data from serial port
app.get('/api/serial/read/:connectionId', (req, res) => {
    try {
        const { connectionId } = req.params;
        const { last = 10 } = req.query;
        
        if (!connectionId || !serialConnections[connectionId]) {
            return res.status(400).json({ 
                success: false, 
                message: 'Invalid connection ID' 
            });
        }

        const conn = serialConnections[connectionId];
        const data = conn.data.slice(-parseInt(last));
        
        res.json({ 
            success: true, 
            data: data,
            isConnected: conn.isConnected
        });
    } catch (error) {
        res.status(500).json({ success: false, message: error.message });
    }
});

// API: Get connection status
app.get('/api/serial/status/:connectionId', (req, res) => {
    try {
        const { connectionId } = req.params;
        
        if (!connectionId || !serialConnections[connectionId]) {
            return res.json({ 
                success: true,
                isConnected: false,
                message: 'No active connection'
            });
        }

        const conn = serialConnections[connectionId];
        res.json({ 
            success: true,
            isConnected: conn.isConnected,
            messageCount: conn.data.length
        });
    } catch (error) {
        res.status(500).json({ success: false, message: error.message });
    }
});

// Serve the web interface
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'web', 'index.html'));
});

// Error handling middleware
app.use((err, req, res, next) => {
    console.error('Server error:', err);
    res.status(500).json({ 
        success: false, 
        message: 'Internal server error',
        error: err.message 
    });
});

// Start server
app.listen(PORT, () => {
    console.log(`\n🚀 ArduinoMaze Server running at http://localhost:${PORT}`);
    console.log(`\n📡 Serial Communication API:`);
    console.log(`   - POST /api/serial/connect - Connect to Arduino`);
    console.log(`   - POST /api/serial/disconnect - Disconnect from Arduino`);
    console.log(`   - POST /api/serial/send - Send data to Arduino`);
    console.log(`   - GET /api/serial/read/:connectionId - Read data from Arduino`);
    console.log(`   - GET /api/serial/ports - List available serial ports`);
    console.log(`   - GET /api/serial/status/:connectionId - Check connection status\n`);
});

// Handle graceful shutdown
process.on('SIGINT', () => {
    console.log('\nShutting down gracefully...');
    // Close all serial connections
    Object.keys(serialConnections).forEach(connectionId => {
        const conn = serialConnections[connectionId];
        if (conn.isConnected) {
            conn.port.close();
        }
    });
    process.exit(0);
});

let playerPos = { row: 0, col: 0 }; // Track player position

let idleAnimationInterval = null;
let animationFrame = 0;
let mazeWallConfig = []; // Store wall configuration for each cell
let selectedPos = null; // currently selected cell for actions (row,col)


let mazeBuilded = false; // Flag to indicate if the maze has been built
let mazeDataArray = []; // Temporary array to hold incoming maze data bytes

const markerCellPos = { row: 7, col: 7 }; // 8x8 cell in 1-based coordinates

function ensureMarkerCell() {
    const markerCell = document.getElementById(`cell-${markerCellPos.row}-${markerCellPos.col}`);
    if (!markerCell) {
        return;
    }

    markerCell.textContent = 'X';
    markerCell.style.textAlign = 'center';
    markerCell.style.alignContent = 'center';
    markerCell.style.fontWeight = 'bold';
    markerCell.style.color = '#45e24a';
}


function isPlayerAtCell(row, col) {
    return playerPos.row === row && playerPos.col === col;
}

function isValidCell(row, col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

function canMoveToCell(row, col) {
    if (!isValidCell(row, col)) {
        return false;
    }

    const currentCell = mazeWallConfig?.[playerPos.row]?.[playerPos.col];
    if (!currentCell) {
        return true;
    }

    if (row === playerPos.row - 1 && col === playerPos.col) {
        return Boolean(currentCell.up);
    }

    if (row === playerPos.row + 1 && col === playerPos.col) {
        return Boolean(currentCell.down);
    }

    if (row === playerPos.row && col === playerPos.col - 1) {
        return Boolean(currentCell.left);
    }

    if (row === playerPos.row && col === playerPos.col + 1) {
        return Boolean(currentCell.right);
    }

    return false;
}

function byteToCells(byte) {
    if (typeof byte !== 'number' || byte < 0 || byte > 255) {
        throw new Error('byte must be an integer between 0 and 255');
    }

    const lowerNibble = byte & 0x0F;
    const upperNibble = (byte >> 4) & 0x0F;

    const decodeNibble = (nibble) => ({
        left: Boolean(nibble & 0x08),
        up:    Boolean(nibble & 0x04),
        right: Boolean(nibble & 0x02),
        down:  Boolean(nibble & 0x01)
    });
    
    return [decodeNibble(upperNibble), decodeNibble(lowerNibble)];
}


function buildMazeFromData(dataArray) {
    if (!Array.isArray(dataArray) || dataArray.length !== 32) {
        throw new Error('dataArray must contain exactly 32 integers (0..255)');
    }

    const mazeGrid = Array.from({ length: 8 }, () => Array(8).fill(null));

    // k -> row = Math.floor(k / 4), columns = (k % 4) * 2 и +1
    for (let k = 0; k < 32; k++) {
        const byteValue = dataArray[k];
        if (typeof byteValue !== 'number' || byteValue < 0 || byteValue > 255) {
            throw new Error(`Invalid byte at index ${k}: ${byteValue}`);
        }

        const row = Math.floor(k / 4);
        const colLeft = (k % 4) * 2;
        const [leftCell, rightCell] = byteToCells(byteValue);

        mazeGrid[row][colLeft] = leftCell;
        mazeGrid[row][colLeft + 1] = rightCell;
    }

    return mazeGrid;
}

function renderMazeWithBorders(mazeData) {
    const mazeScreen = document.querySelector('.maze_screen');
    mazeScreen.innerHTML = '';

    checkMazeWalls(mazeData);
    mazeWallConfig = mazeData;
    for (let i = 0; i < 8; i++) {
        for (let j = 0; j < 8; j++) {
            const cell = document.createElement('div');
            cell.className = 'maze_cell';
            cell.id = `cell-${i}-${j}`;
            cell.dataset.row = i;
            cell.dataset.col = j;
            cell.style.margin = '2px';

            const wallConfig = mazeData[i][j];
            const wallBorders = [];
            
            if (!wallConfig.up) wallBorders.push('inset 0 3px 0 rgb(21, 146, 42)');
            if (!wallConfig.left) wallBorders.push('inset 3px 0 0 rgb(21, 146, 42)');
            if (!wallConfig.right) wallBorders.push('inset -3px 0 0 rgb(21, 146, 42)');
            if (!wallConfig.down) wallBorders.push('inset 0 -3px 0 rgb(21, 146, 42)');

            if (wallBorders.length > 0) {
                cell.style.boxShadow = wallBorders.join(', ');
            }

            // Click selects the cell for rotation/actions (does not move the player)
            cell.addEventListener('click', () => {
                setSelectedCell(i, j);
            });

            mazeScreen.appendChild(cell);
        }
    }

    // Set initial player position and selector position
    updatePlayerDisplay();
    // Initialize selector at player position if not set
    if (!selectedPos) {
        selectedPos = { row: playerPos.row, col: playerPos.col };
    }
    updateSelectedDisplay();
    ensureMarkerCell();

    // Stop idle animation when maze is loaded
    stopIdleAnimation();
}

function checkMazeWalls(mazeData) {
    if (!Array.isArray(mazeData) || mazeData.length !== 8) {
        return;
    }

    for (let row = 0; row < 8; row++) {
        for (let col = 0; col < 8; col++) {
            const cell = mazeData[row]?.[col];
            if (!cell) {
                continue;
            }

            cell.up = Boolean(cell.up);
            cell.down = Boolean(cell.down);
            cell.left = Boolean(cell.left);
            cell.right = Boolean(cell.right);
        }
    }
}

// Initialize 8x8 maze grid
function initializeMazeGrid() {
    const mazeScreen = document.querySelector('.maze_screen');
    mazeScreen.innerHTML = '';
    
    for (let i = 0; i < 8; i++) {
        for (let j = 0; j < 8; j++) {
            const cell = document.createElement('div');
            cell.className = 'maze_cell';
            cell.id = `cell-${i}-${j}`;
            cell.dataset.row = i;
            cell.dataset.col = j;
            cell.style.margin = '2px';
            // Click selects the cell for rotation/actions (does not move the player)
            cell.addEventListener('click', function() {
                setSelectedCell(i, j);
            });

            mazeScreen.appendChild(cell);
        }
    }
    
    updatePlayerDisplay();
    // Initialize selector at player position if not set
    if (!selectedPos) {
        selectedPos = { row: playerPos.row, col: playerPos.col };
    }
    updateSelectedDisplay();
    ensureMarkerCell();
    
    startIdleAnimation();
}

function movePlayerTo(row, col) {
    if (!isValidCell(row, col)) {
        return false;
    }

    playerPos.row = row;
    playerPos.col = col;
    updatePlayerDisplay();
    return true;
}

function setSelectedCell(row, col) {
    if (!isValidCell(row, col)) return false;
    selectedPos = { row, col };
    updateSelectedDisplay();
    if (typeof logSerialOutput === 'function') {
        logSerialOutput(`➤ Selected cell ${row},${col}`);
    }
    return true;
}

function updateSelectedDisplay() {
    // Clear previous inline outlines
    document.querySelectorAll('.maze_cell').forEach(cell => {
        cell.style.backgroundColor = '';
    });

    if (!selectedPos) return;
    const el = document.getElementById(`cell-${selectedPos.row}-${selectedPos.col}`);
    if (el) {
        el.style.backgroundColor = '#fa9a6d';
    }
}

// Rotate selected cell
function rotateCell(row, col) {
    if (!Array.isArray(mazeWallConfig) || mazeWallConfig.length !== 8) {
        return false;
    }

    if (!isValidCell(row, col)) {
        return false;
    }

    if (isPlayerAtCell(row, col)) {
        if (typeof logSerialOutput === 'function') {
            logSerialOutput(`✗ Cannot rotate occupied cell at ${row},${col}`);
        }
        return false;
    }

    const cell = mazeWallConfig[row]?.[col];
    if (!cell) {
        return false;
    }

    const rotated = {
        right: cell.up,
        down: cell.right,
        left: cell.down,
        up: cell.left
    };

    mazeWallConfig[row][col] = rotated;
    // Keep directional/asymmetric walls as-is; do not force neighbor symmetry.
    renderMazeWithBorders(mazeWallConfig);
    return true;
}

// Update player display on grid
function updatePlayerDisplay() {
    document.querySelectorAll('.maze_cell').forEach(cell => {
        cell.classList.remove('player');
    });
    
    // Add player class to current position
    const playerCell = document.getElementById(`cell-${playerPos.row}-${playerPos.col}`);
    if (playerCell) {
        playerCell.classList.add('player');
    }
}

// Start idle animation (gradient wave)
function startIdleAnimation() {
    if (idleAnimationInterval) return; // Already running
    
    animationFrame = 0;
    idleAnimationInterval = setInterval(() => {
        const cells = document.querySelectorAll('.maze_cell');
        
        cells.forEach((cell, index) => {
            const hue = (index * 12 + animationFrame * 8) % 360;
            const saturation = 75;
            const lightness = 50;
            
            cell.style.background = `hsl(${hue}, ${saturation}%, ${lightness}%)`;
            cell.style.transition = 'background 0.15s ease-in-out';
        });
        
        animationFrame++;
    }, 120);
}

// Stop idle animation
function stopIdleAnimation() {
    if (idleAnimationInterval) {
        clearInterval(idleAnimationInterval);
        idleAnimationInterval = null;
        
        // Reset cell colors to default styles
        document.querySelectorAll('.maze_cell').forEach(cell => {
            cell.style.background = '';
            cell.style.transition = '';
        });
        
        updatePlayerDisplay();
        
        document.querySelectorAll('.maze_cell.wall').forEach(cell => {
            cell.style.background = '#4a4a4a';
        });
        document.querySelectorAll('.maze_cell.goal').forEach(cell => {
            cell.style.background = '#4CAF50';
        });
    }
}

// Initialize grid when page loads
document.addEventListener('DOMContentLoaded', initializeMazeGrid);

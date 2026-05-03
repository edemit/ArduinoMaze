let maze = [[]]; // array of arrays in bytes 
let playerPos = { row: 0, col: 0 }; // Track player position

let idleAnimationInterval = null;
let animationFrame = 0;
let mazeWallConfig = []; // Store wall configuration for each cell

let mazeBuilded = false; // Flag to indicate if the maze has been built
let mazeDataArray = []; // Temporary array to hold incoming maze data bytes

function byteToCells(byte) {
    if (typeof byte !== 'number' || byte < 0 || byte > 255) {
        throw new Error('byte must be an integer between 0 and 255');
    }

    const lowerNibble = byte & 0x0F;
    const upperNibble = (byte >> 4) & 0x0F;

    const decodeNibble = (nibble) => ({
        right: Boolean(nibble & 0x08),
        up:    Boolean(nibble & 0x04),
        left:  Boolean(nibble & 0x02),
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
        // 41 = 0010 1001 que le bas qui est bon
        // 
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

            const wallConfig = mazeData[i][j];
            const wallBorders = [];
            
            if (!wallConfig.up) wallBorders.push('inset 0 3px 0 #333');
            if (!wallConfig.left) wallBorders.push('inset -3px 0 0 #333');
            if (!wallConfig.right) wallBorders.push('inset 3px 0 0 #333');
            if (!wallConfig.down) wallBorders.push('inset 0 -3px 0 #333');

            if (wallBorders.length > 0) {
                cell.style.boxShadow = wallBorders.join(', ');
            }

            cell.addEventListener('click', () => {
                movePlayerTo(i, j);
            });

            mazeScreen.appendChild(cell);
        }
    }

    // Set initial player position
    updatePlayerDisplay();

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

            if (col < 7 && mazeData[row][col + 1]) {
                const rightNeighbor = mazeData[row][col + 1];
                const sharedOpen = Boolean(cell.right && rightNeighbor.left);
                cell.right = sharedOpen;
                rightNeighbor.left = sharedOpen;
            }

            if (row < 7 && mazeData[row + 1][col]) {
                const bottomNeighbor = mazeData[row + 1][col];
                const sharedOpen = Boolean(cell.down && bottomNeighbor.up);
                cell.down = sharedOpen;
                bottomNeighbor.up = sharedOpen;
            }
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
            
            cell.addEventListener('click', function() {
                movePlayerTo(i, j);
            });
            
            mazeScreen.appendChild(cell);
        }
    }
    
    updatePlayerDisplay();
    
    startIdleAnimation();
}

function movePlayerTo(row, col) {
    playerPos.row = row;
    playerPos.col = col;
    updatePlayerDisplay();
}

function movePlayerDirection(direction) {
    let newRow = playerPos.row;
    let newCol = playerPos.col;
    
    switch(direction) {
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
    }
    
    movePlayerTo(newRow, newCol);
}

// Rotate selected cell
function rotateCell(row, col) {
    if (!Array.isArray(mazeWallConfig) || mazeWallConfig.length !== 8) {
        return;
    }

    if (row < 0 || row > 7 || col < 0 || col > 7) {
        return;
    }

    const cell = mazeWallConfig[row]?.[col];
    if (!cell) {
        return;
    }

    const rotated = {
        right: cell.up,
        down: cell.right,
        left: cell.down,
        up: cell.left
    };

    mazeWallConfig[row][col] = rotated;
    checkNeighborWalls(row, col);

    renderMazeWithBorders(mazeWallConfig);
}

function checkNeighborWalls(row, col) {
    const cell = mazeWallConfig[row][col];

    // Up neighbor
    if (row > 0) {
        mazeWallConfig[row - 1][col].down = cell.up;
    }

    // Right neighbor
    if (col < 7) {
        mazeWallConfig[row][col + 1].left = cell.right;
    }

    // Down neighbor
    if (row < 7) {
        mazeWallConfig[row + 1][col].up = cell.down;
    }

    // Left neighbor
    if (col > 0) {
        mazeWallConfig[row][col - 1].right = cell.left;
    }
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
            cell.style.background = '#000';
        });
        document.querySelectorAll('.maze_cell.goal').forEach(cell => {
            cell.style.background = '#4CAF50';
        });
    }
}

// Initialize grid when page loads
document.addEventListener('DOMContentLoaded', initializeMazeGrid);
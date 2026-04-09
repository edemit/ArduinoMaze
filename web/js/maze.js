let maze = [[]]; // array of arrays in bytes 
let playerPos = { row: 0, col: 0 }; // Track player position

let idleAnimationInterval = null;
let animationFrame = 0;

// Initialize 8x8 maze grid
function initializeMazeGrid() {
    const mazeScreen = document.querySelector('.maze_screen');
    mazeScreen.innerHTML = ''; // Clear existing content
    
    for (let i = 0; i < 8; i++) {
        for (let j = 0; j < 8; j++) {
            const cell = document.createElement('div');
            cell.className = 'maze_cell';
            cell.id = `cell-${i}-${j}`;
            cell.dataset.row = i;
            cell.dataset.col = j;
            
            // Add click handler for mouse movement
            cell.addEventListener('click', function() {
                movePlayerTo(i, j);
            });
            
            mazeScreen.appendChild(cell);
        }
    }
    
    // Set initial player position
    updatePlayerDisplay();
    
    // Start idle animation on page load
    startIdleAnimation();
}

// Move player to a specific cell (local only, no serial)
function movePlayerTo(row, col) {
    playerPos.row = row;
    playerPos.col = col;
    updatePlayerDisplay();
}

// Move player in a direction (local only, no serial)
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

// Update player display on grid
function updatePlayerDisplay() {
    // Remove player class from all cells
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
// index.js

import os from 'os';
import bindings from 'bindings';

let libgpiod;
if (os.type() === 'Linux') {
    // entry point
    libgpiod = bindings('node-libgpiod');
    libgpiod.Chip.prototype.getLine = function getLine(n) {
        return new libgpiod.Line(this, n);
    };
    
    libgpiod.available = function() {
        return true;
    }
} else {
    libgpiod = {
        available: function() {
            return false;
        }
    }
}
export const {Chip, Line} = libgpiod;
export default libgpiod;
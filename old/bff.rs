// This version is abandoned, for now
use std::io::Read;

const TAPESIZE: usize = 30000;

fn main() {
    let mut tape: [i32;TAPESIZE] = [0;TAPESIZE];
    let mut exe: usize = 0; // execution pointer
    let mut ptr: usize = 0; // pointer for data manipulation
    let mut rstack: Vec<usize> = Vec::new(); // stack for recursion pointers
    let mut stack: Vec<i32> = Vec::new(); // Stack for program use

    // Get code
    let code = "\\h>\\e>\\l>\\l>\\o>\\\n<<<<<[.>]";
    let code: Vec<char> = code.chars().collect();

    // iterate over code and copy into tape
    for (i, c) in code.iter().enumerate() {
        tape[i] = *c as i32;
    };
    
    // set data pointer to just after code
    ptr = code.len() + 1;

    loop {
        match tape[exe] as u8 as char {
            '\\' => { exe += 1; tape[ptr] = tape[exe]}, //copies next char in exe to data pointer
            '>' => ptr += 1,
            '<' => ptr -= 1,
            '+' => tape[ptr] += 1,
            '-' => tape[ptr] -= 1,
            '[' => rstack.push(exe + 1),
            ']' => if tape[ptr] == 0 { rstack.pop(); exe += 1; continue; } else { exe = rstack[rstack.len() - 1]; continue },
            '.' => if tape[ptr] < 256 { let c = tape[ptr] as u8 as char ; print!("{}", c) },
            ',' => {
                    let mut input: [u8; 1] = [0; 1];
                    let _ = std::io::stdin().read_exact(&mut input);
                    tape[ptr] = input[0] as i32;
            },

            '\0' => break, // ends program
            _ => break,
        }

        // increment the execute pointer
        exe += 1;

        // if execute pointer is out of the code then break
        if exe >= code.len() { break; } ;
    }

}

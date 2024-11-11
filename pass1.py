import re

# Define the opcode table with tuple format
opcode_table = {
    'STOP': ('IS', '00'),
    'ADD': ('IS', '01'),
    'SUB': ('IS', '02'),
    'MULT': ('IS', '03'),
    'MOVER': ('IS', '04'),
    'MOVEM': ('IS', '05'),
    'COMP': ('IS', '06'),
    'BC': ('IS', '07'),
    'DIV': ('IS', '08'),
    'READ': ('IS', '09'),
    'PRINT': ('IS', '10'),
    'START': ('AD', '01'),
    'END': ('AD', '02'),
    'ORIGIN': ('AD', '03'),
    'EQU': ('AD', '04'),
    'LTORG': ('AD', '05'),
    'DS': ('DL', '01'),
    'DC': ('DL', '02'),
    'AREG': ('RG', '01'),
    'BREG': ('RG', '02'),
    'CREG': ('RG', '03'),
    'DREG': ('RG', '04'),
    'EQ': ('CC', '01'),
    'LT': ('CC', '02'),
    'GT': ('CC', '03'),
    'LE': ('CC', '04'),
    'GE': ('CC', '05'),
    'NE': ('CC', '06')
}

# Initialize tables
symbol_table = {}  # {symbol: (address, value)}
literal_table = []  # [(literal_value, address)]
pool_table = []
intermediate_code = []

def get_register_code(operand):
    if operand in opcode_table:
        return opcode_table[operand]
    return None

def handle_literal(literal, loc_counter):
    """Process literal and return its index"""
    value = literal.strip('=')
    if literal not in [l[0] for l in literal_table]:
        literal_table.append([literal, None])
    return ('L', len(literal_table) - 1)

def process_symbol(symbol, loc_counter):
    """Process symbol and return its index"""
    if symbol not in symbol_table:
        symbol_table[symbol] = loc_counter
    return ('S', list(symbol_table.keys()).index(symbol))

def process_line(line, loc_counter):
    if not line.strip():
        return loc_counter
    
    parts = line.strip().split()
    label = ""
    operands = []
    
    if len(parts) == 3:
        label, opcode, operand = parts
        operands = operand.split(',')
    elif len(parts) == 2:
        opcode, operand = parts
        operands = operand.split(',')
    else:
        opcode = parts[0]
    
    # Handle label
    if label and label not in opcode_table:
        symbol_table[label] = loc_counter
    
    # Process instruction
    if opcode in opcode_table:
        ic_entry = []
        ic_entry.append(opcode_table[opcode])  # Add opcode tuple
        
        # START instruction
        if opcode == 'START':
            ic_entry.append(('C', int(operands[0]) if operands else 0))
            intermediate_code.append(tuple(ic_entry))
            return int(operands[0]) if operands else 0
        
        # END instruction
        elif opcode == 'END':
            intermediate_code.append(tuple(ic_entry))
            # Handle pending literals
            for lit in literal_table:
                if lit[1] is None:
                    lit[1] = loc_counter
                    intermediate_code.append((('DL', '01'), ('C', 1), loc_counter))
                    loc_counter += 1
            return loc_counter
        
        # LTORG instruction
        elif opcode == 'LTORG':
            pool_table.append(len(literal_table))
            for lit in literal_table:
                if lit[1] is None:
                    lit[1] = loc_counter
                    intermediate_code.append((('DL', '01'), ('C', 1), loc_counter))
                    loc_counter += 1
            return loc_counter
        
        # Process operands
        if operands:
            for operand in operands:
                if operand.startswith('='):  # Literal
                    ic_entry.append(handle_literal(operand, loc_counter))
                elif operand in opcode_table:  # Register
                    ic_entry.append(opcode_table[operand])
                else:  # Symbol
                    ic_entry.append(process_symbol(operand, loc_counter))
        
        # Add location counter for instructions that need it
        if opcode_table[opcode][0] in ['IS', 'DL']:
            ic_entry.append(loc_counter)
        
        intermediate_code.append(tuple(ic_entry))
        
        # Update location counter
        if opcode == 'DS':
            loc_counter += int(operands[0]) if operands else 1
        elif opcode != 'EQU':
            loc_counter += 1
    
    return loc_counter

def process_file(input_file):
    loc_counter = 0
    
    # First pass
    with open(input_file, 'r') as file:
        for line in file:
            loc_counter = process_line(line.strip(), loc_counter)
    
    # Write intermediate code
    with open('intermediate_code.txt', 'w') as file:
        for entry in intermediate_code:
            file.write(f"{str(entry)}\n")
    
    # Write symbol table
    with open('symbol_table.txt', 'w') as file:
        for symbol, addr in symbol_table.items():
            file.write(f"{symbol}\t{addr}\n")
    
    # Write literal table
    with open('literal_table.txt', 'w') as file:
        for i, (literal, addr) in enumerate(literal_table):
            file.write(f"{literal}\t{addr}\n")
    
    # Write pool table
    with open('pool_table.txt', 'w') as file:
        for i, pool_size in enumerate(pool_table):
            file.write(f"#{i+1}\t{pool_size}\n")

# Test the assembler
if __name__ == "__main__":
    process_file('testCases1.txt')
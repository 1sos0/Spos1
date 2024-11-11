import ast

# 
def read_intermediate_code(filename):
    """Read intermediate code from file"""
    intermediate_code = []
    with open(filename, 'r') as file:
        for line in file:
            try:
                # Strip parentheses and split by comma
                line = line.strip().strip('()')
                if not line:
                    continue
                    
                # Parse components
                parts = line.split(',')
                # Convert to proper tuple format
                if len(parts) == 1:
                    intermediate_code.append((parts[0],))
                elif len(parts) == 2:
                    intermediate_code.append((parts[0], parts[1]))
                elif len(parts) == 3:
                    intermediate_code.append((parts[0], parts[1], parts[2]))
                    
            except (ValueError, SyntaxError) as e:
                print(f"Warning: Skipping invalid line: {line}")
                continue
    return intermediate_code

def read_symbol_table(filename):
    """Read symbol table from file"""
    symbol_table = {}
    with open(filename, 'r') as file:
        for line in file:
            if line.strip():
                try:
                    symbol, address = line.strip().split('\t')
                    symbol_table[symbol] = int(address)
                except ValueError:
                    print(f"Warning: Invalid symbol table entry: {line}")
                    continue
    return symbol_table

def read_literal_table(filename):
    """Read literal table from file"""
    literal_table = []
    with open(filename, 'r') as file:
        for line in file:
            if line.strip():
                literal, address = line.strip().split('\t')
                literal_table.append((literal, int(address)))
    return literal_table

def read_pool_table(filename):
    """Read pool table from file"""
    pool_table = []
    with open(filename, 'r') as file:
        for line in file:
            if line.strip():
                _, size = line.strip().split('\t')
                pool_table.append(int(size))
    return pool_table

def generate_machine_code(ic_entry, symbol_table, literal_table):
    """Generate machine code for a single instruction"""
    
    # Extract the components
    opcode = ic_entry[0]
    
    # Handle different instruction types
    if opcode[0] == 'IS':
        # Machine code for imperative statements
        machine_code = [opcode[1]]  # Add opcode
        
        # Add register or condition code if present
        if len(ic_entry) > 1:
            if ic_entry[1][0] in ['RG', 'CC']:
                machine_code.append(ic_entry[1][1])
            else:
                machine_code.append('0')
        
        # Add address field if present
        if len(ic_entry) > 2:
            if ic_entry[2][0] == 'S':  # Symbol
                machine_code.append(str(symbol_table[list(symbol_table.keys())[ic_entry[2][1]]]))
            elif ic_entry[2][0] == 'L':  # Literal
                machine_code.append(str(literal_table[ic_entry[2][1]][1]))
            else:  # Constant
                machine_code.append(str(ic_entry[2][1]))
        
        return '+' + ' '.join(machine_code)
    
    elif opcode[0] == 'DL':
        if opcode[1] == '01':  # DS
            return f"+ 00 0 {ic_entry[1][1]}"  # Default initialization
        elif opcode[1] == '02':  # DC
            return f"+ 00 0 {ic_entry[1][1]}"
        
    elif opcode[0] == 'AD':
        return ''  # Assembler directives don't generate machine code
    
    return ''

def pass2():
    try:
        # Read all tables
        intermediate_code = read_intermediate_code('intermediate_code.txt')
        symbol_table = read_symbol_table('symbol_table.txt')
        literal_table = read_literal_table('literal_table.txt')
        pool_table = read_pool_table('pool_table.txt')
        
        # Generate machine code
        machine_code = []
        for entry in intermediate_code:
            mc = generate_machine_code(entry, symbol_table, literal_table)
            if mc:  # Only add non-empty machine code
                if len(entry) > 3:  # If location counter is present
                    machine_code.append(f"{entry[-1]}\t{mc}")
                else:
                    machine_code.append(mc)
        
        # Write machine code to file
        with open('machine_code.txt', 'w') as file:
            for code in machine_code:
                if code.strip():  # Only write non-empty lines
                    file.write(f"{code}\n")
                    
        print("Machine code generated successfully!")
        
    except FileNotFoundError as e:
        print(f"Error: Required input file not found - {e.filename}")
    except Exception as e:
        print(f"Error during pass 2: {str(e)}")

# Additional utility functions for specific machine code formats
def format_register_operand(reg_code):
    """Format register operands"""
    return str(reg_code).zfill(1)

def format_address(address):
    """Format addresses in machine code"""
    return str(address).zfill(3)

# Run pass 2
if __name__ == "__main__":
    pass2()
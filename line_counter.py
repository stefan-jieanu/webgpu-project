import os

def is_code_file(filename):
    return filename.endswith('.cpp') or filename.endswith('.hpp')

def count_lines_in_file(filepath):
    count = 0
    in_block_comment = False

    with open(filepath, 'r', encoding='utf-8', errors='ignore') as file:
        for line in file:
            stripped = line.strip()
            # if not stripped:
            #     continue  # Skip empty lines

            # if in_block_comment:
            #     if '*/' in stripped:
            #         in_block_comment = False
            #     continue

            # if stripped.startswith('//'):
            #     continue  # Single-line comment

            # if '/*' in stripped:
            #     in_block_comment = True
            #     continue  # Start of block comment

            count += 1  # Count the line as code

    return count

def count_lines_in_project(root_dir):
    total = 0
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if is_code_file(filename):
                full_path = os.path.join(dirpath, filename)
                file_lines = count_lines_in_file(full_path)
                print(f"Counted {file_lines} lines in: {full_path}")
                total += file_lines
    return total

if __name__ == '__main__':
    root = './src'
    total_lines = count_lines_in_project(root)
    print(f"\nYay, we have ʕっ•ᴥ•ʔっ: {total_lines}, ")
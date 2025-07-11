import random
import string

def generate_random_hash(length=64, prefix="FL1TZ{", suffix="}"):
    charset = string.ascii_lowercase + string.digits  # common in hashes
    random_hash = ''.join(random.choices(charset, k=length))
    return f"{prefix}{random_hash}{suffix}"

# Example usage
if __name__ == "__main__":
    length = 64  # Length of the hash portion
    print(generate_random_hash(length))


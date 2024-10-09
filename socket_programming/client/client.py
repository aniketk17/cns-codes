import socket
import time
import struct

def connect_to_server(host, port, max_attempts=5, delay=2):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    for attempt in range(max_attempts):
        try:
            print(f"🔄 [Client]: Attempting to connect to the server on {host}:{port} (Attempt {attempt + 1}/{max_attempts})...")
            client_socket.connect((host, port))
            print("✅ [Client]: Connected to the server!\n")
            return client_socket
        except ConnectionRefusedError:
            print(f"❌ [Client]: Connection failed. Retrying in {delay} seconds...")
            time.sleep(delay)
    print("❌ [Client]: Failed to connect after multiple attempts. Please check the server status.")
    return None

def receive_file(client_socket, filename):
    try:
        with open(f"downloaded_{filename}", 'wb') as file:
            print("📂 [Client]: Receiving file...")
            
            # Receive file size first
            file_size_bytes = client_socket.recv(8)
            (file_size,) = struct.unpack('!Q', file_size_bytes)
            
            received_size = 0
            start_time = time.time()

            while received_size < file_size:
                chunk = client_socket.recv(min(4096, file_size - received_size))
                if not chunk:
                    raise RuntimeError("Connection closed unexpectedly")
                file.write(chunk)
                received_size += len(chunk)
                
                # Update progress
                progress = (received_size / file_size) * 100
                elapsed_time = time.time() - start_time
                speed = received_size / (1024 * 1024 * elapsed_time)  # MB/s
                print(f"\r📥 [Client]: Received {received_size}/{file_size} bytes ({progress:.2f}%) - {speed:.2f} MB/s", end="", flush=True)

        print(f"\n✅ [Client]: File '{filename}' received successfully!")
    except Exception as e:
        print(f"\n❌ [Client]: Error during file transfer: {e}")

def send_message(client_socket, message):
    try:
        client_socket.send(message.encode('utf-8'))
    except socket.error as e:
        print(f"❌ [Client]: Error sending message: {e}")
        return False
    return True

def receive_message(client_socket):
    try:
        return client_socket.recv(1024).decode('utf-8')
    except socket.error as e:
        print(f"❌ [Client]: Error receiving message: {e}")
        return None

def main():
    host = 'localhost'
    port = 3000
    
    client_socket = connect_to_server(host, port)
    if not client_socket:
        return

    try:
        while True:
            print("\n🔍 [Client]: Choose an option:")
            print("1. Say Hello")
            print("2. File Transfer")
            print("3. Calculator")
            print("4. List Server Files")
            print("5. Exit")
            
            option = input("📥 [Client]: Enter your choice: ")
            
            if not send_message(client_socket, option):
                break
            
            if option == '1':
                print("📩 [Client]: Sending 'Say Hello' request to the server...")
                message = receive_message(client_socket)
                if message:
                    print(f"📤 [Server]: {message}")
            
            elif option == '2':
                print("📩 [Client]: Initiating File Transfer...")
                filename = input("📥 [Client]: Enter the filename to request from the server: ")
                if send_message(client_socket, filename):
                    response = receive_message(client_socket)
                    if response == "File found":
                        receive_file(client_socket, filename)
                    else:
                        print(f"❌ [Client]: {response}")
            
            elif option == '3':
                print("📩 [Client]: Accessing Calculator...")
                expression = input("🧮 [Client]: Enter a mathematical expression (e.g., 5 + 3 * 2): ")
                if send_message(client_socket, expression):
                    result = receive_message(client_socket)
                    if result:
                        print(f"📤 [Server]: {result}")
            
            elif option == '4':
                print("📩 [Client]: Requesting list of server files...")
                response = receive_message(client_socket)
                if response:
                    print("📁 [Server Files]:")
                    print(response)
            
            elif option == '5':
                print("🔌 [Client]: Disconnecting from the server...")
                break
            
            else:
                print("❌ [Client]: Invalid option. Please try again.")
    
    except KeyboardInterrupt:
        print("\n🛑 [Client]: Operation interrupted by user.")
    finally:
        client_socket.close()
        print("❌ [Client]: Connection closed.")

if __name__ == "__main__":
    main()
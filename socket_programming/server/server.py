import socket
import os
import struct

def start_server(host, port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"🚀 [Server]: Started on {host}:{port}")
    return server_socket

def send_file(client_socket, filename):
    try:
        with open(filename, 'rb') as file:
            
            file_size = os.path.getsize(filename)   
            
            # Send file size first
            client_socket.send(struct.pack('!Q', file_size))
            
            # Send file content in chunks
            bytes_sent = 0
            while bytes_sent < file_size:
                chunk = file.read(4096)
                if not chunk:
                    break
                client_socket.send(chunk)
                bytes_sent += len(chunk)
                
                # Print progress
                progress = (bytes_sent / file_size) * 100
                print(f"\r📤 [Server]: Sent {bytes_sent}/{file_size} bytes ({progress:.2f}%)", end="", flush=True)
            
        print(f"\n✅ [Server]: File '{filename}' sent successfully!")
    except Exception as e:
        print(f"❌ [Server]: Error sending file: {e}")
        client_socket.send("Error occurred while sending the file".encode('utf-8'))

def handle_client(client_socket):
    while True:
        option = client_socket.recv(1024).decode('utf-8')
        
        if option == '1':
            print("📩 [Server]: Received 'Say Hello' request")
            client_socket.send("👋 Hello from the server!".encode('utf-8'))
        
        elif option == '2':
            print("📩 [Server]: Received File Transfer request")
            filename = client_socket.recv(1024).decode('utf-8')
            if os.path.exists(filename):
                client_socket.send("File found".encode('utf-8'))
                send_file(client_socket, filename)
            else:
                client_socket.send(f"File '{filename}' not found".encode('utf-8'))
        
        elif option == '3':
            print("📩 [Server]: Received Calculator request")
            expression = client_socket.recv(1024).decode('utf-8')
            try:
                result = eval(expression)
                client_socket.send(f"Result: {result}".encode('utf-8'))
            except Exception as e:
                client_socket.send(f"Error: {str(e)}".encode('utf-8'))
        
        elif option == '4':
            print("📩 [Server]: Received request for file list")
            files = "\n".join(os.listdir('.'))
            client_socket.send(files.encode('utf-8'))
        
        elif option == '5':
            print("🔌 [Server]: Client requested to exit")
            break
        
        else:
            print(f"❓ [Server]: Received unknown option: {option}")
    
    client_socket.close()
    print("❌ [Server]: Client disconnected")

def main():
    host = 'localhost'
    port = 3000
    
    server_socket = start_server(host, port)
    
    try:
        while True:
            print("🔄 [Server]: Waiting for a connection...")
            client_socket, addr = server_socket.accept()
            print(f"✅ [Server]: Connected to client at {addr}")
            handle_client(client_socket)
    except KeyboardInterrupt:
        print("\n🛑 [Server]: Server stopped by user")
    finally:
        server_socket.close()
        print("❌ [Server]: Server shut down")

if __name__ == "__main__":
    main()
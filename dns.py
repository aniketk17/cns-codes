import subprocess

def nslookup(domain_or_ip):
    try:
        result = subprocess.check_output(['nslookup', domain_or_ip])
        print(result.decode())
    except subprocess.CalledProcessError as e:
        print(f"Error occurred: {e}")

while True:
    print("\nMenu:")
    print("1. Lookup IP Address from URL using nslookup")
    print("2. Lookup Domain Name from IP Address using nslookup")
    print("3. Exit")
    
    choice = input("Enter your choice (1/2/3): ")
    
    if choice == '1':
        url = input('Enter URL: ')
        nslookup(url)
    
    elif choice == '2':
        ip = input('Enter IP Address: ')
        nslookup(ip)
    
    elif choice == '3':
        print("Exiting...")
        break
    
    else:
        print("Invalid choice, please select a valid option.")

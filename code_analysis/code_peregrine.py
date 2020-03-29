# code to connect to peregrine
import paramiko

try:
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect("peregrine.hpc.rug.nl", username="someuser", password="somepassword")
    stdin, stdout, stderr = client.exec_command("ls")
    stdout = list(map(lambda x: x.strip("\n"), stdout.readlines()))
    print(stdout)

finally:
    client.close()

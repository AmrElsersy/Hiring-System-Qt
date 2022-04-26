import  socket
import json, time
from urllib import response

HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 8080  # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

    conn = s.connect(HOST, PORT)

        print(f"Connected by {addr}")
        while True:
            print("Wait to receive ...")
            data = conn.recv(1024).decode()
            if not data:
                print("Disconnected ! Wait again ...")
                conn, addr = s.accept()
                data = conn.recv(1024).decode()

            print(data, type(data))

            # text to json object
            json_data = json.loads(data)
            print("json data: ", json_data)

            response = {}
            if json_data["type"] == "jobs":
                response["type"] = "jobs"
                response["jobs"] = [
                    {
                        "name":"MED Software",
                        "requirements":
                            """
                            - C++
                            - OOP
                            - Algorithms
                            - Datastructures
                            - Qt
                            - Software Engineering
                            """},

                    {"name": "Computer Vision Com Iot", "requirements":
                        """
                        - Tensorflow or Pytorch
                        - ROS Robot Operating System
                        - Qt
                        - TensorRT and CUDA
                        - C++ and Python
                        """}]

            elif json_data["type"] == "submit":
                response["type"] = "feedback"
                response["feedback"] = "THIS IS A FEEDBACK ON THE JOB APPLICATION AND YOU ARE REJECTED HAHA"

            # convert json to text
            response_bytes = json.dumps(response)
            # text to bytes and send them
            conn.sendall(response_bytes.encode('utf-8'))


from flask import Flask, request, jsonify

app = Flask(__name__)

#Variables globales
cont_disp = 0 #Contador de espacios disponibles
cont_reserv = 0 #Contador de espacios reservados
cont_ocup = 0 #Contador de espacios ocupados
barrera_entrada = 0
alarma_activa = False
#verde: 1
#amarillo: 2
#rojo: 3

#Creando el parqueo de 2 niveles con 16 espacios cada uno
#[[1,1,1,1,..],[1,1,1,..]]
parqueo = []
for i in range(2):
    parqueo.append([])
    for j in range(8):
        parqueo[i].append(1)

@app.route("/")
def hello_world():
    return "<p>PROYECTO 1 GRUPO 8, ARQUI 1 </p>"
    
@app.route("/marcador", methods=['GET'])
def marcadorEspacios():
    global cont_disp 
    global cont_ocup 
    global cont_reserv 

    ##Reiniciando contadores
    cont_disp = 0
    cont_ocup = 0
    cont_reserv = 0

    #Verificando espacio por espacio del parqueo y aumentando contadores
    for nivel in parqueo:
        for espacio in nivel:
            if espacio == 1:
                cont_disp+=1
            elif espacio == 2:
                cont_reserv+=1
            elif espacio == 3:
                cont_ocup+=1

    marcador = {
        "Espacios disponibles": cont_disp,
        "Espacios ocupados": cont_ocup,
        "Espacios reservados": cont_reserv
    }
    
    return jsonify(marcador)

@app.route("/estadoBarrera", methods=['GET'])
def abrirBarrera_entrada():
    estado = 0
    barrera = ""
    if cont_disp == 0:
        barrera = {
            "Mensaje": "La barrera no se abrirá porque ya no hay espacios disponibles",
            "Estado": 0
        }
    else:
        barrera = {
            "Mensaje": "La barrera se abrirá porque aún hay espacios disponibles",
            "Estado": 1
        }

    return jsonify(barrera)

@app.route("/abrirBarreraSalida", methods=['GET'])
def abrirBarrera_salida():
    barrera = {
        "Estado": 1 #La barrera de salida siempre se puede abrir
    }
    return jsonify(barrera)

'''
#JSON A RECIBIR:
{
    "nivel": 1 o 2,
    "posicion": 1 al 16
}
'''
@app.route("/reservarEspacio", methods=['POST'])
def reservarEspacio():
    data = request.json
    response = ""
    estado = parqueo[data["nivel"]-1][data["posicion"]-1]

    if estado == 1:
        parqueo[data["nivel"]-1][data["posicion"]-1] = 2
        response = {
            "Mensaje": "Se reservó con éxito el espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"]),
            "Estado": 1
        }
    elif estado == 2:
        response = {
            "Mensaje": "El espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"])+" ya ha sido reservado",
            "Estado": 0
        }
        
    elif estado == 3:
        response = {
            "Mensaje":"El espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"])+" ya está ocupado",
            "Estado":0
        }

    return jsonify(response)
   
#se mira en proteus
@app.route("/ocuparEspacio", methods=['POST'])
def ocuparEspacio():
    data = request.json
    response = ""
    estado = parqueo[data["nivel"]-1][data["posicion"]-1]

    if estado == 1:
        parqueo[data["nivel"]-1][data["posicion"]-1] = 3
        response = {
            "Mensaje": "Se ocupó con éxito el espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"]),
            "Estado": 1
        }
    elif estado == 2:
        response = {
            "Mensaje": "El espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"])+" ya ha sido reservado",
            "Estado": 0
        }
        
    elif estado == 3:
        response = {
            "Mensaje":"El espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"])+" ya está ocupado",
            "Estado":0
        }

    return jsonify(response)


@app.route("/espacioOcupado", methods=['GET'])
def EspacioOcupado():
    global parqueo
    espacios = []
    for i in range(2):
        for j in range(8):
            if(parqueo[i][j]) == 1:
                if i == 0:
                    if j == 0:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "A1",
                            "Alarma": 0
                        })
                    elif j == 1:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "A2",
                            "Alarma": 0
                        })
                    elif j == 2:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "A3",
                            "Alarma": 0
                        })
                    elif j == 3:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "A4",
                            "Alarma": 0
                        })
                    elif j == 4:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "B1",
                            "Alarma": 0
                        })
                    elif j == 5:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "B2",
                            "Alarma": 0
                        })
                    elif j == 6:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "B3",
                            "Alarma": 0
                        })
                    elif j == 7:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "B4",
                            "Alarma": 0
                        })
                elif i == 1:
                    if j == 0:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "C1",
                            "Alarma": 0
                        })
                    elif j == 1:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "C2",
                            "Alarma": 0
                        })
                    elif j == 2:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "C3",
                            "Alarma": 0
                        })
                    elif j == 3:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "C4",
                            "Alarma": 0
                        })
                    elif j == 4:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "D1",
                            "Alarma": 0
                        })
                    elif j == 5:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "D2",
                            "Alarma": 0
                        })
                    elif j == 6:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "D3",
                            "Alarma": 0
                        })
                    elif j == 7:
                        espacios.append({
                            "estado": 1,
                            "parqueo": "D4",
                            "Alarma": 0
                        })
            elif(parqueo[i][j]) == 2:
                if i == 0:
                    if j == 0:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "A1",
                            "Alarma": 0
                        })
                    elif j == 1:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "A2",
                            "Alarma": 0
                        })
                    elif j == 2:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "A3",
                            "Alarma": 0
                        })
                    elif j == 3:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "A4",
                            "Alarma": 0
                        })
                    elif j == 4:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "B1",
                            "Alarma": 0
                        })
                    elif j == 5:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "B2",
                            "Alarma": 0
                        })
                    elif j == 6:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "B3",
                            "Alarma": 0
                        })
                    elif j == 7:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "B4",
                            "Alarma": 0
                        })
                elif i == 1:
                    if j == 0:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "C1",
                            "Alarma": 0
                        })
                    elif j == 1:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "C2",
                            "Alarma": 0
                        })
                    elif j == 2:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "C3",
                            "Alarma": 0
                        })
                    elif j == 3:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "C4",
                            "Alarma": 0
                        })
                    elif j == 4:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "D1",
                            "Alarma": 0
                        })
                    elif j == 5:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "D2",
                            "Alarma": 0
                        })
                    elif j == 6:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "D3",
                            "Alarma": 0
                        })
                    elif j == 7:
                        espacios.append({
                            "estado": 2,
                            "parqueo": "D4",
                            "Alarma": 0
                        })
            elif(parqueo[i][j]) == 3:
                if i == 0:
                    if j == 0:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "A1",
                            "Alarma": 0
                        })
                    elif j == 1:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "A2",
                            "Alarma": 0
                        })
                    elif j == 2:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "A3",
                            "Alarma": 0
                        })
                    elif j == 3:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "A4",
                            "Alarma": 0
                        })
                    elif j == 4:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "B1",
                            "Alarma": 0
                        })
                    elif j == 5:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "B2",
                            "Alarma": 0
                        })
                    elif j == 6:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "B3",
                            "Alarma": 0
                        })
                    elif j == 7:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "B4",
                            "Alarma": 0
                        })
                elif i == 1:
                    if j == 0:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "C1",
                            "Alarma": 0
                        })
                    elif j == 1:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "C2",
                            "Alarma": 0
                        })
                    elif j == 2:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "C3",
                            "Alarma": 0
                        })
                    elif j == 3:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "C4",
                            "Alarma": 0
                        })
                    elif j == 4:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "D1",
                            "Alarma": 0
                        })
                    elif j == 5:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "D2",
                            "Alarma": 0
                        })
                    elif j == 6:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "D3",
                            "Alarma": 0
                        })
                    elif j == 7:
                        espacios.append({
                            "estado": 3,
                            "parqueo": "D4",
                            "Alarma": 0
                        })
    return jsonify(espacios)

#EN PROTEUS
@app.route("/desocuparEspacio", methods=['POST'])
def desocuparEspacio():
    data = request.json
    response = ""
    estado = parqueo[data["nivel"]-1][data["posicion"]-1]

    if estado == 1:
        response = {
            "Mensaje": "El espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"])+" ya está desocupado",
            "Estado": 0
        }
    elif estado == 2:
        response = {
            "Mensaje": "El espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"])+" ya ha sido reservado",
            "Estado": 0
        }
        
    elif estado == 3:
        parqueo[data["nivel"]-1][data["posicion"]-1] = 1
        response = {
            "Mensaje":"Se desocupó con éxito el espacio "+str(data["posicion"])+" del nivel "+str(data["nivel"]),
            "Estado":1
        }

    return jsonify(response)

'''
JSON A RECIBIR:
{
    "nivel": 1 o 2,
    "posicion": 1 al 16
}
'''
@app.route("/alarmaAntirrobos", methods=['POST'])
def AlarmaAntirrobos():
    data = request.json
    response = ""
    estado = parqueo[data["nivel"]-1][data["posicion"]-1]
    if estado == 3:
        if data["Antirrobo"] == 1:	
            response = {
                "Mensaje": "Se activó la alarma antirrobos",
                "Antirrobo": 1
            }
        elif data["Antirrobo"] == 0:
            response = {
                "Mensaje": "No se activó la alarma antirrobos",
                "Antirrobo": 0
            }
    else:
        response = {
            "Mensaje": "No se activó la alarma antirrobos",
            "Antirrobo": 0
        }
    return jsonify(response)

'''
JSON:
{
    "nivel": 1 o 2,
    "posicion": 1 al 16
}
'''

@app.route("/alarmaParqueo", methods=['POST'])
def AlarmaParqueo():
    data = request.json
    response = ""
    estado = parqueo[data["nivel"]-1][data["posicion"]-1]
    if estado == 1 and data["Antirrobo"] == 1:
        response = {
            "Mensaje": "Se activó la alarma del parqueo",
            "Alarma": 1
        }
    elif estado == 1 and data["Antirrobo"] == 0:
        response = {
            "Mensaje": "No se activó la alarma del parqueo",
            "Alarma": 0
        }
    else:
        response = {
            "Mensaje": "No se activó la alarma del parqueo",
            "Alarma": 0
        }
    return jsonify(response)



if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)




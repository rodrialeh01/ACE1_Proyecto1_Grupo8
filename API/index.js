const express = require('express');
const app = express();
//var SerialPort = require("serialport");
const { SerialPort } = require('serialport')

var port = 3000;
var arduinoCOMPort = "COM2";

var arduinoSerialPort = new SerialPort({ 
    path: arduinoCOMPort,
    baudRate: 9600
});

arduinoSerialPort.on('open', function() {
    console.log('El Puerto Serial ' + arduinoCOMPort + ' está abierto.');
});

let cont_disp = 0;
let cont_reserv = 0;
let cont_ocup = 0;
let barrera_entrada = 0;
let alarma_activa = false;

let parqueo = [[], []];
let alarmas = [[], []];

for (var i=0; i<2; i++){
    for(var j=0; j<8; j++){
        parqueo[i][j] = 1;
        alarmas[i][j] = 0;
    }
}


app.use(express.json());

app.get('/', (req,res) => {
    res.send('API');
});

app.get('/arduino/:action', function (req, res) {
    
    var action = req.params.action || req.param('action');
     
     if(action == 'led'){
         arduinoSerialPort.write("w");
         return res.send('Led light is on!');
     } 
     if(action == 'off') {
         arduinoSerialPort.write("t");
         return res.send("Led light is off!");
     }
     
     return res.send('Action: ' + action);
  
 });

app.get('/api/students', (req,res) => {
    res.send
})

app.get('/marcador', (req,res) => {
    cont_disp = 0
    cont_ocup = 0
    cont_reserv = 0

    for(var i = 0; i < parqueo.length; i++) {
        for(var j = 0; j < parqueo[i].length; j++) {
            if (parqueo[i][j] == 1){
                cont_disp++
            }else if(parqueo[i][j] == 2){
                cont_reserv++
            }else if(parqueo[i][j] == 3){
                cont_ocup++
            }
        }
    }

    res.json({
        "Espacios disponibles": cont_disp,
        "Espacios ocupados": cont_ocup,
        "Espacios reservados": cont_reserv
    })

})

app.get('/estadoBarrera', (req, res) => {
    if(cont_disp == 0){
        res.json({
            "Mensaje": "La barrera no se abrirá porque ya no hay espacios disponibles",
            "Estado": 0
        })
    }
    else{
        res.json({
            "Mensaje": "La barrera se abrirá porque aún hay espacios disponibles",
            "Estado": 1
        })
    }
})

app.get('/abrirBarreraSalida',(req,res) => {
    res.json({
        "Estado": 1
    })
})

app.post('/reservarEspacio',(req,res) => {
    data = req.body;
    response = "";
    estado = parqueo[data.nivel-1][data.posicion-1];
    if(estado == 1){
        parqueo[data.nivel-1][data.posicion-1] = 2;
        response = {
            "Estado": 1
        }
    }else if(estado == 2){
        parqueo[data.nivel-1][data.posicion-1] = 1;
        response = {
            "Estado": 0
        }
    }else if(estado == 3){
        response = {
            "Estado":0
        }
    }
    res.json(response);
})

app.post('/ocuparEspacio',(req,res) => {
    data = req.body;
    response = "";
    estado = parqueo[data.nivel-1][data.posicion-1];
    if(estado == 1){
        parqueo[data.nivel-1][data.posicion-1] = 3;
        console.log(parqueo[data.nivel-1][data.posicion-1])
        response = {
            "Mensaje": "Se ocupó con éxito el espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString(),
            "Estado": 1
        }
    }else if(estado == 2){
        response = {
            "Mensaje": "El espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString()+" ya ha sido reservado",
            "Estado": 0
        }
    }else if(estado == 3){
        response = {
            "Mensaje": "El espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString()+" ya está ocupado",
            "Estado": 0
        }
    }
    res.json(response);
})

app.get('/espacioOcupado', (req,res) => {
    espacios = [];
    estacionamientoEstado = [];
    i=0;
    arduinoSerialPort.write("Z");
    contador = 0;
    while(i<1000){
        caracter = arduinoSerialPort.read();
        if (caracter!=null){
            console.log(caracter.toString());
            Estado = caracter.toString();

            estacionamientoEstado[contador]=Estado;
            contador++;
        }
        i++;
    }
    //parqueo1 = 0
    //parqueo2 = 1 replace /n,""
    //
    console.log("Estos son los estados: ")
    for(var u = 0; u<estacionamientoEstado.length; u++){
        console.log(estacionamientoEstado[u]);
    }

    for(var i = 0; i < parqueo.length; i++) {
        for(var j = 0; j < parqueo[i].length; j++) {
            if(parqueo[i][j] == 1){
                if(i == 0){
                    if(j == 0){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "A1",
                            "Alarma": 0
                        })
                    }else if( j == 1){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "A2",
                            "Alarma": 0
                        })
                    }else if (j == 2){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "A3",
                            "Alarma": 0
                    })
                    }else if (j == 3){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "A4",
                            "Alarma": 0
                    })
                    }else if (j == 4){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "B1",
                            "Alarma": 0
                    })
                    }else if (j == 5){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "B2",
                            "Alarma": 0
                    })
                    }else if (j == 6){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "B3",
                            "Alarma": 0
                    })
                    }else if (j == 7){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "B4",
                            "Alarma": 0
                    })}
                }else if (i == 1)
                    if (j == 0){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "C1",
                            "Alarma": 0
                        })
                    }else if (j == 1){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "C2",
                            "Alarma": 0
                        })
                    }else if (j == 2){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "C3",
                            "Alarma": 0
                        })
                    }else if (j == 3){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "C4",
                            "Alarma": 0
                        })
                    }else if (j == 4){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "D1",
                            "Alarma": 0
                        })
                    }else if (j == 5){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "D2",
                            "Alarma": 0
                        })
                    }else if (j == 6){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "D3",
                            "Alarma": 0
                        })
                    }else if (j == 7){
                        espacios.push({
                            "estado": 1,
                            "parqueo": "D4",
                            "Alarma": 0
                        })}
            }else if(parqueo[i][j] == 2){
                if (i == 0){
                    if (j == 0){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "A1",
                            "Alarma": 0
                        })
                    }else if (j == 1){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "A2",
                            "Alarma": 0
                        })
                    }else if (j == 2){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "A3",
                            "Alarma": 0
                        })
                    }else if (j == 3){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "A4",
                            "Alarma": 0
                        })
                    }else if (j == 4){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "B1",
                            "Alarma": 0
                        })
                    }else if (j == 5){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "B2",
                            "Alarma": 0
                        })
                    }else if (j == 6){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "B3",
                            "Alarma": 0
                        })
                    }else if (j == 7){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "B4",
                            "Alarma": 0
                        })}
                }else if (i == 1){
                    if (j == 0){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "C1",
                            "Alarma": 0
                        })
                    }else if (j == 1){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "C2",
                            "Alarma": 0
                        })
                    }else if (j == 2){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "C3",
                            "Alarma": 0
                        })
                    }else if (j == 3){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "C4",
                            "Alarma": 0
                        })
                    }else if (j == 4){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "D1",
                            "Alarma": 0
                        })
                    }else if (j == 5){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "D2",
                            "Alarma": 0
                        })
                    }else if (j == 6){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "D3",
                            "Alarma": 0
                        })
                    }else if (j == 7){
                        espacios.push({
                            "estado": 2,
                            "parqueo": "D4",
                            "Alarma": 0
                        })
                    }
            }else if(parqueo[i][j] == 3){
                if (i == 0){
                    if (j == 0){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "A1",
                            "Alarma": 0
                        })
                    }else if (j == 1){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "A2",
                            "Alarma": 0
                        })
                    }else if (j == 2){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "A3",
                            "Alarma": 0
                        })
                    }else if (j == 3){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "A4",
                            "Alarma": 0
                        })
                    }else if (j == 4){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "B1",
                            "Alarma": 0
                        })
                    }else if (j == 5){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "B2",
                            "Alarma": 0
                        })
                    }else if (j == 6){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "B3",
                            "Alarma": 0
                        })
                    }else if (j == 7){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "B4",
                            "Alarma": 0
                        })}
                }else if (i == 1){
                    if (j == 0){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "C1",
                            "Alarma": 0
                        })
                    }else if (j == 1){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "C2",
                            "Alarma": 0
                        })
                    }else if (j == 2){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "C3",
                            "Alarma": 0
                        })
                    }else if (j == 3){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "C4",
                            "Alarma": 0
                        })
                    }else if (j == 4){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "D1",
                            "Alarma": 0
                        })
                    }else if (j == 5){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "D2",
                            "Alarma": 0
                        })
                    }else if (j == 6){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "D3",
                            "Alarma": 0
                        })
                    }else if (j == 7){
                        espacios.push({
                            "estado": 3,
                            "parqueo": "D4",
                            "Alarma": 0
                        })
                    }
                }
                }
            }
        }
    }

    res.contentType('application/json');
    res.send(JSON.stringify(espacios));

})

app.post('/desocuparEspacio',(req,res) => {
    data = req.body;
    response = "";
    estado = parqueo[data.nivel-1][data.posicion-1];
    if(estado == 1){
        response = {
            "Mensaje": "El espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString()+" ya está desocupado",
            "Estado": 0
        }
    }else if(estado == 2){
        response = {
            "Mensaje": "El espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString()+" ya ha sido reservado",
            "Estado": 0
        }
    }else if(estado == 3){
        parqueo[data.nivel-1][data.posicion-1] = 1;
        response = {
            "Mensaje": "Se desocupó con éxito el espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString(),
            "Estado": 1
        }
    }
    res.json(response);
})

app.post('/alarmaAntirrobos',(req,res) => {
    data = req.body;
    response = "";
    estado = alarmas[data.nivel-1][data.posicion-1];
    if(estado == 0){
        alarmas[data.nivel-1][data.posicion-1] = 1;
        response = {
            "Mensaje": "Se activó la alarma del espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString(),
            "Estado": 1
        }
    }else if(estado == 1){
        alarmas[data.nivel-1][data.posicion-1] = 0;
        response = {
            "Mensaje": "Se desactivó la alarma del espacio "+data.posicion.toString()+" del nivel "+data.nivel.toString(),
            "Estado": 0
        }
    }
    res.json(response);
})

app.post('/alarmaParqueo',(req,res) => { 
    data = req.body;
    response = "";
    estado = alarmas[data.nivel-1][data.posicion-1];
    if(estado == 1 && data.Antirrobo==1){
        response = {
            "Mensaje": "Se activó la alarma del espacio ",
            "Alarma": 1
        }
    }else if(estado == 1 && data.Antirrobo==0){
        response = {
            "Mensaje": "Se desactivó la alarma del espacio ",
            "Alarma": 0
        }
    }else{
        response = {
            "Mensaje": "No se activó la alarma del espacio ",
            "Alarma": 0
        }
    }
    res.json(response);
})

//const port = process.env.port || 8080;
app.listen(port, () => console.log("Escuchando en el puerto "+port));

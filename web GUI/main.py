from flask import Flask                 //python web package
from flask import render_template
import folium                           //map drawing package
import boto3                            //local connect server
from readfile import readDB

app = Flask(__name__)

// folium colorList 
colorList = ['red', 'blue', 'green', 'purple']

@app.route("/")
def map_marker():
    colorDict = {}
    fmap = folium.Map(
         location=[data[-1]["Message"]["Lat"], data[-1]["Message"]["Lng"]],
         tiles='Stamen Terrain',
         zoom_start=15
    )
    c = 0
    for d in newData:
        if d["Device_Name"] not in colorDict:
            colorDict[d["Device_Name"]] = colorList[c]
            c += 1
        t = str(d["Time"])
        date = t[0:4] + "/" + t[4:6] + "/" + t[6:8]
        time = t[8:10] + ":" + t[10:12] + ":" + t[12:14]
        folium.Marker(
            location=[d["Message"]["Lat"], d["Message"]["Lng"]],
            popup="<b>" + "<center>" + "Name: " + d["Device_Name"] + "<br>" + date + "<br>" + "<center>" + time + "</b>",
            icon=folium.Icon(icon="cube", color=colorDict[d["Device_Name"]], prefix='fa')

        ).add_to(fmap)

    folium.LayerControl().add_to(fmap)

    fmap.save('container_tracking.html')

    return fmap._repr_html_()

if __name__ == "__main__":

    // Some privacy items were hid. You need the dynamodb API ID, KEY and table name to access your table on AWS.

    response = table.scan()

    data = response['Items']
    
    // import readfile.py; there are 2 modes to read the table in Dynamodb
    r = readDB()
    
    // Mode 1 : enter the time interval and the devices name, you can see the container routes in that time interval
    #devices = ["003", "004"]
    #newData = r.timeInterval(devices, "2022/01/06,21:50", "2022/01/06,22:10", data)
    
    // Mode 2 : display the containers location lately
    newData = r.current(data)

    app.run(debug=True)

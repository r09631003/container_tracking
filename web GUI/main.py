from flask import Flask
from flask import render_template
import folium
import boto3
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

    //AWS API ID & KEY
    ACCESS_ID = 'YOUR_ACCESS_ID'
    ACCESS_KEY = 'YOUR_ACCESS_KEY'
    dynamodb = boto3.resource('dynamodb', region_name='YOUR_REGION', aws_access_key_id=ACCESS_ID,
                              aws_secret_access_key=ACCESS_KEY)
    
    table = dynamodb.Table('YOUR_DYNAMODB_TABLE_NAME')

    response = table.scan()

    data = response['Items']

    r = readDB()
    
    // Mode 1 : enter the time interval and the devices name, you can see the container routes in that time interval
    #devices = ["003", "004"]
    #newData = r.timeInterval(devices, "2022/01/06,21:50", "2022/01/06,22:10", data)
    
    // Mode 2 : display the containers location lately
    newData = r.current(data)

    app.run(debug=True)

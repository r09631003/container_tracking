class readDB:
    def __init__(self):
        self.data = []
        self.dict = set()

    def current(self, raw):
        raw.reverse()

        for r in raw:
            if r["Device_Name"] not in self.dict:
                self.dict.add(r["Device_Name"])
                self.data.append(r)

            else:
                continue

        return self.data

    def timeInterval(self, devices, startTime, endTime, data):
        s = startTime.split(",")
        sDate = s[0].split("/")
        sTime = s[1].split(":")

        sD = int(sDate[0])*pow(10, 10) + int(sDate[1])*pow(10, 8) + int(sDate[2])*pow(10, 6)+\
             int(sTime[0])*pow(10, 4) + int(sTime[1])*pow(10, 2)

        e = endTime.split(",")
        eDate = e[0].split("/")
        eTime = e[1].split(":")
        eD = int(eDate[0]) * pow(10, 10) + int(eDate[1]) * pow(10, 8) + int(eDate[2])*pow(10, 6) + \
             int(eTime[0]) * pow(10, 4) + int(eTime[1]) * pow(10, 2) + 59

        if eD < sD:
            return -1

        for d in data:
            if d["Device_Name"] not in devices:
                continue

            if d["Time"] > sD and d["Time"] < eD:
                self.data.append(d)

        return self.data

from flask import Flask, request, jsonify


app = Flask(__name__)


# Store the current motor command
# direction: -1=reverse, 0=stop, 1=forward
# speed: 0..100 (percentage)
motorVal = {"direction": 0, "speed": 0}


@app.route('/motorVal', methods=['GET', 'POST'])
def motor_value():
    global motorVal

    if request.method == 'POST':
        data = request.get_json(silent=True)

        if not data:
            return jsonify({"error": "Expected JSON body"}), 400

        # Validate presence
        if "direction" not in data or "speed" not in data:
            return jsonify({"error": "JSON must include 'direction' and 'speed'"}), 400

        # Validate values
        try:
            direction = int(data["direction"])
            speed = float(data["speed"])
        except (ValueError, TypeError):
            return jsonify({"error": "direction must be int; speed must be number"}), 400

        #if direction not in (-1, 0, 1):
        #    return jsonify({"error": "direction must be -1, 0, or 1"}), 400
        if not (0 <= speed <= 100):
            return jsonify({"error": "speed must be between 0 and 100"}), 400

        motorVal = {"direction": direction, "speed": speed}
        return jsonify({"status": "ok", "motorVal": motorVal})

    # GET returns current value
    return jsonify({"motorVal": motorVal})
@app.route('/updateDir', methods=['POST'])
def updateDir():
    global motorVal
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON body received"}), 400
    
    direction = data.get("direction")
    if not direction:
        return jsonify({"error": "Missing 'direction' field"}), 400
    
    motorVal["direction"] = data.get("direction")
    print(motorVal["direction"])
    return jsonify({"status": "success", "direction": direction})



if __name__ == '__main__':
    # Bind to all interfaces; use a non-privileged port
    app.run(host='0.0.0.0', port=8080)

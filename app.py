import subprocess
from flask import Flask, render_template, request

app = Flask(__name__)

@app.route("/")
def home():
    return render_template("index.html")

@app.route("/run", methods=["POST"])
def run_program():
    # Get lists of payers, receivers, and amounts from the form
    payers = request.form.getlist("payer")
    receivers = request.form.getlist("receiver")
    amounts = request.form.getlist("amount")

    # Combine the lists into the format expected by the C program
    program_input = f"{len(payers)}\n"
    for payer, receiver, amount in zip(payers, receivers, amounts):
        program_input += f"{payer} {receiver} {amount}\n"

    try:
        # Run the compiled C program with the formatted input
        result = subprocess.check_output(
            ["./cashflow.exe"],  # Use .exe for Windows
            input=program_input,
            text=True,
            encoding='utf-8',  # <-- This is the fix for the Unicode error
        )
    except subprocess.CalledProcessError as e:
        result = f"Error running program:\n{e.output}"
    except FileNotFoundError:
        result = "Error: cashflow.exe not found. Make sure it's compiled and in the same directory."

    return render_template("index.html", output=result)

if __name__ == "__main__":
    app.run(debug=True)
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import login_required

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database-----------0-------------UNDONE!!!!!!!!
db = SQL("sqlite:///users.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/")
def index():
    return render_template("index.html")

@app.route('/signup', methods=['GET','POST'])
def signup():
    if request.method == 'POST':
        Username = request.form.get('username')
        Password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        if Password == confirmation:
            #create new user
            db.execute("INSERT INTO users(Username,Password) VALUES(?,?)", Username, Password)
            flash('New user created')
            return redirect("/")
    else:
        return render_template("signup.html")

@app.route("/login", methods = ['POST','GET'])
def login():
    #forget any user_id
    session.clear()

    if request.method =='POST':
        Username = request.form.get('name')
        Password = request.form.get('password')

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE Username = ?", Username)

        #error message if user does not exist or password invalid
        if len(rows)!= 1 or Password != rows[0]["Password"]:
            flash('Invalid username or ')
            return redirect("/login")

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash('Successfully logged in')
        return redirect("/")

    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to home
    return redirect("/")

@app.route("/tutorials")
@login_required
def tutorials():
    return render_template("tutorials.html")

@app.route("/about")
def about():
    return render_template("about.html")

@app.route("/contact")
def contact():
    return render_template("contact.html")

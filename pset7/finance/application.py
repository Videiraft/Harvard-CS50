from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    userId=session["user_id"]
    # select the portfolio for the current user
    stocks = db.execute("SELECT stock, totalShares FROM portfolio WHERE id = :uId", uId=userId)
    # total value of all stocks
    stocksValor = 0
    
    for stock in stocks:
        # look up for the missing information
        stockInfo = lookup(stock["stock"])
        # add it to the original info on the database (to the dictionary 'stocks')
        stock["name"] = stockInfo["name"]
        stock["price"] = stockInfo["price"]
        stockValor = stockInfo["price"] * stock["totalShares"]
        stocksValor = stocksValor + stockValor
        stock["total"] = stockValor
    
    user = db.execute("SELECT * FROM users WHERE id = :uId", uId=userId)
    cash = user[0]["cash"]
    total = cash + stocksValor

    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        uId=session["user_id"]
        # save the number of shares to buy, if valid, into a variable
        try:
            nShares = int(request.form.get("number-of-shares"))
        except:
            return apology("You must input a valid positive number of shares!")
        # Look up for the stock and ensure it is valid and positive
        stock = lookup(request.form.get("stock-symbol"))
        if not stock:
            return apology("This stock's symbol is not valid!")
        if nShares < 0:
            return apology("You must buy a positive number of shares!")
        
        # ensure the user has suficient funds
        totalPrice = stock["price"] * nShares
        cashUser = db.execute("SELECT cash FROM users WHERE id = :uId", uId=uId)
        if cashUser[0]["cash"] - totalPrice < 0:
            return apology("The user cannot afford this number of shares at the current price.")
        
        # Add the new purchase to the history table
        db.execute("INSERT INTO history (id, stock, shares, price, type) VALUES (:uId, :stock, :shares, :price, 'BUY')", 
                    uId = uId, stock = stock["symbol"], shares = nShares, price = stock["price"])
        
        # update the user cash
        db.execute("UPDATE users SET cash = cash - :tPrice WHERE id = :uId", tPrice = totalPrice, uId = uId)
        
        # update the portfolio database or insert a new stock
        update = db.execute("UPDATE portfolio SET totalShares = totalShares + :totalShares WHERE id = :uId AND stock = :stock", 
                totalShares=nShares, uId=uId, stock=stock["symbol"])
        if not update:
            db.execute("INSERT INTO portfolio (id, stock, totalShares) VALUES (:uId, :stock, :totalShares)", uId=uId, 
                stock=stock["symbol"], totalShares=nShares)
        
        return redirect(url_for("index"))
        
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    uId = session["user_id"]
    history = db.execute("SELECT * FROM history WHERE id = :uId", uId = uId)
    
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Look up for the stock
        quote = lookup(request.form.get("stock-symbol"))
        # Ensure the stock symbol is valid 
        if not quote:
            return apology("This stock's symbol is not valid!")
        
        return render_template("quoted.html", symbol=quote["symbol"], name=quote["name"], price=quote["price"])
        
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Missing username!")
        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("Missing password!")
        else:
            user = request.form.get("username")
        # Check if both password fields match
        if not request.form.get("password") == request.form.get("password-confirmation"):
            return apology("Password and password confirmation don't match!")
            
        # check if the user already exists
        result = db.execute("SELECT username FROM users WHERE username = :user", user=user)
        if result != []:
            return apology("The username {} is not available! Please choose another one.".format(user))
                
        # Register the user on the database
        hashPass = pwd_context.hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashP)", username=user, hashP=hashPass)
        
        # Log in the user
        userID = db.execute("SELECT id FROM users WHERE username = :username", username=user)
        session["user_id"] = userID[0]["id"]
        
        # redirect user to home page
        return redirect(url_for("index"))
    
    # if user reached route via POST
    else:    
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        # ensure the stock symbol is valid
        stock = request.form.get("stock-symbol")
        if not stock:
            return apology("This stock's symbol is not valid!")

        # ensure the user inputs a valid number of shares to sell
        uId = session["user_id"]
        try:
            nShares = int(request.form.get("number-of-shares"))
        except:
            return apology("You must input a valid positive number of shares!")
            
        totalShares = db.execute("SELECT totalShares FROM portfolio WHERE id = :uId AND stock = :stock", uId=uId, stock=stock)
        if nShares < 1 or nShares > totalShares[0]["totalShares"]:
            return apology("The number of shares is not valid!")

        # Update Portfolio
        db.execute("UPDATE portfolio SET totalShares = totalShares - :nShares WHERE stock = :stock", nShares = nShares, stock = stock)
        
        # Update cash's user
        stockInfo = lookup(stock)
        sellValor = stockInfo["price"] * float(nShares)
        db.execute("UPDATE users SET cash = cash + :sellValor WHERE id = :uId", sellValor = sellValor, uId = uId)
        
        # Add transaction to history
        db.execute("INSERT INTO history (id, stock, shares, price, type) VALUES (:uId, :stock, :shares, :price, 'SELL')", 
                    uId = uId, stock = stock, shares = nShares, price = stockInfo["price"])
        
        return redirect(url_for("index"))
        
    else:
        return render_template("sell.html")
        

@app.route("/changePwd", methods=["GET", "POST"])
@login_required
def changePwd():
    if request.method == "POST":
        """Change the user's password"""
        uId = session["user_id"]
         # ensure password was submitted
        if not request.form.get("new-password"):
            return apology("Missing new password!")
                
        # Check if both password fields match
        if not request.form.get("new-password") == request.form.get("confirm-new-password"):
            return apology("Password and password confirmation don't match!")
        
        # create new hash
        hashPass = pwd_context.hash(request.form.get("new-password"))
        
        # update database with new hash
        db.execute("UPDATE users SET hash = :hashPass WHERE id = :uId", uId=uId, hashPass=hashPass)
        
        return redirect(url_for("index"))
    
    else:
        return render_template("changePwd.html")
#!/usr/bin/env python3
import os
import sys

from flask import Flask, redirect, render_template, request, url_for

import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    try:
        tweets = helpers.get_user_timeline(screen_name, 100)
    except Exception as e:
        sys.exit("Exception has been raised: %s" % e)
    if tweets == None:
        sys.exit("Couldn't get tweets")
    
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # Analyzing tweets
    analyzer = Analyzer(positives, negatives)

    scores = {
        "positive" : 0,
        "negative" : 0,
        "neutral" : 0
    }

    for tweet in tweets:
        score = analyzer.analyze(tweet)
        if score > 0.0:
            scores["positive"] += 1
        elif score < 0.0:
            scores["negative"] += 1
        else:
            scores["neutral"] += 1
    
    total_score = 0
    for key in scores:
        total_score += scores[key]
    
    positive = scores["positive"]
    negative = scores["negative"]
    neutral = scores["neutral"]

    print("positive: {}, negative: {}, neutral: {}".format(scores["positive"],scores["negative"],scores["neutral"]))
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)

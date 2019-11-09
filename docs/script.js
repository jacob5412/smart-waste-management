// Your web app's Firebase configuration
let firebaseConfig = {
    apiKey: "",
    authDomain: "",
    databaseURL: "",
    projectId: "",
    storageBucket: "",
    messagingSenderId: "",
    appId: "",
    measurementId: ""
};
firebase.initializeApp(firebaseConfig);

// Get a reference to the database service
let smartbins = firebase.database().ref('smartbin');

// display all bins
smartbins.orderByChild('timestamp').once("value").then(function (snapshot) {
    // Get the bin data from the most recent snapshot of data
    // added to the bin list in Firebase
    snapshot.forEach(function (childSnapshot) {
        smartbin = childSnapshot.val();
        // Update the HTML to display the bin text
        $("#displayTable").append("<tr><td>" 
        + smartbin.id + "</td><td>" 
        + smartbin.timestamp + "</td><td>" 
        + smartbin.filllevel + "</td><td>" 
        + smartbin.temp + "</td><td>" 
        + smartbin.hum + "</td><td>" 
        + smartbin.topple + "</td></tr>")
    })
});

// Save a new bin to the database, using the input in the form
var submitSmartbin = function () {

    var binid = $("#binid").val();
    var filllevel = $("#filllevel").val();
    var humidity = $("#humidity").val();
    var temperature = $("#temperature").val();
    var topple = $("#topple").val();
    var timestamp = $("#timestamp").val();

    // Push a new bin to the database using those values
    smartbins.push({
        "id": binid,
        "filllevel": filllevel,
        "hum": humidity,
        "temp": temperature,
        "topple": topple,
        "timestamp": timestamp
    });
};

// Get the single most recent bin from the database and
// update the table with its values. This is called every time the child_added
// event is triggered on the smartbin Firebase reference, which means
// that this will update EVEN IF you don't refresh the page. Magic.
smartbins.limitToLast(1).on('child_added', function (childSnapshot) {
    // Get the bin data from the most recent snapshot of data
    // added to the bin list in Firebase
    smartbin = childSnapshot.val();

    // Update the HTML to display the bin text
    $("#binshow").html(smartbin.id)
    $("#timeshow").html(smartbin.timestamp)
    $("#fillshow").html(smartbin.filllevel)
    $("#humshow").html(smartbin.hum)
    $("#tempshow").html(smartbin.temp)
    $("#toppleshow").html(smartbin.topple)
});

// When the window is fully loaded, call this function.
// Note: because we are attaching an event listener to a particular HTML element
// in this function, we can't do that until the HTML element in question has
// been loaded. Otherwise, we're attaching our listener to nothing, and no code
// will run when the submit button is clicked.
$(window).load(function () {

    // Find the HTML element with the id addData, and when the submit
    // event is triggered on that element, call submitSmartbin.
    $("#addData").submit(submitSmartbin);

});
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Slider.H> 
#include <FL/Fl_Box.H>    
#include <FL/fl_draw.H>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#define _USE_MATH_DEFINES


class Pen {
protected:
    Fl_Color color; // Pen's color
    int thickness;  // Pen's thickness

public:
    Pen(Fl_Color c = FL_BLACK) : color(c), thickness(1) {}

    void setThickness(int t) { thickness = t; } // Set thickness
    int getThickness() const { return thickness; } // Get thickness

    void setColor(Fl_Color c) { color = c; } // Set color
    Fl_Color getColor() const { return color; } // Get color

    // Draw a line with the specified thickness and color
    virtual void drawLine(int x1, int y1, int x2, int y2) {
        fl_color(color);
        fl_line_style(FL_SOLID, thickness);
        fl_line(x1, y1, x2, y2);
        fl_line_style(FL_SOLID, 1); // Reset to default thickness
    }

    virtual ~Pen() {}
};

class ColorfulPen : public Pen {
    std::vector<Fl_Color> colors; // A collection of colors to cycle through
    size_t currentColorIndex;     // Tracks the current color

public:
    ColorfulPen() : Pen(FL_RED), currentColorIndex(0) {
        colors = {FL_RED, FL_GREEN, FL_BLUE}; // Initialize with three colors
    }

    // Draw a line and cycle to the next color
    void drawLine(int x1, int y1, int x2, int y2) override {
        color = colors[currentColorIndex];
        Pen::drawLine(x1, y1, x2, y2);
        currentColorIndex = (currentColorIndex + 1) % colors.size();
    }
};

// Abstract base class for all shapes
class Shape {
protected:
    Pen* pen; // Pen used to draw the shape

public:
    Shape(Pen* p) : pen(p) {}
    virtual void draw() = 0; // Pure virtual function for drawing the shape
    virtual ~Shape() {}
};

// Represents a line segment
class Line : public Shape {
    int x1, y1, x2, y2; // Coordinates of the line's endpoints

public:
    Line(Pen* p, int x1, int y1, int x2, int y2) : Shape(p), x1(x1), y1(y1), x2(x2), y2(y2) {}

    void draw() override {
        pen->drawLine(x1, y1, x2, y2); // Draw the line using the pen
    }
};

// Represents a rectangle
class Rectangle : public Shape {
    int x, y, width, height; // Top-left corner and dimensions of the rectangle

public:
    Rectangle(Pen* p, int x, int y, int width, int height)
        : Shape(p), x(x), y(y), width(width), height(height) {}

    void draw() override {
        // Draw the four edges of the rectangle
        pen->drawLine(x, y, x + width, y);              // Top edge
        pen->drawLine(x + width, y, x + width, y + height); // Right edge
        pen->drawLine(x + width, y + height, x, y + height); // Bottom edge
        pen->drawLine(x, y + height, x, y);             // Left edge
    }
};

// Represents a circle
class Circle : public Shape {
    int cx, cy, radius; // Center and radius of the circle

public:
    Circle(Pen* p, int cx, int cy, int radius) : Shape(p), cx(cx), cy(cy), radius(radius) {}

    void draw() override {
        const int segments = 100; // Number of line segments used to approximate the circle
        double angleStep = 2 * M_PI / segments; // Angle step for each segment

        // Start at the top of the circle
        int prevX = cx + radius;
        int prevY = cy;

        for (int i = 1; i <= segments; ++i) {
            double angle = i * angleStep;
            int newX = cx + static_cast<int>(radius * cos(angle));
            int newY = cy + static_cast<int>(radius * sin(angle));

            // Connect the previous point to the current point
            pen->drawLine(prevX, prevY, newX, newY);

            prevX = newX;
            prevY = newY;
        }
    }
};



class Canvas : public Fl_Widget {
private:
    std::vector<Shape*> shapes; // Stores all shapes to be drawn on the canvas

public:
    Canvas(int X, int Y, int W, int H, const char* L = 0) : Fl_Widget(X, Y, W, H, L) {}

    // Override draw method to render the canvas and shapes
    void draw() override {
        fl_color(FL_WHITE); // Set background color to white
        fl_rectf(x(), y(), w(), h()); // Fill the canvas background

        fl_color(FL_BLACK); // Set axis color to black
        fl_line(x(), y() + h(), x() + w(), y() + h()); // Draw X-axis
        fl_line(x(), y(), x(), y() + h());             // Draw Y-axis

        // Draw Y-axis labels and ticks
        for (int i = 0; i <= h(); i += 50) {
            int label = 550 - (i / 50) * 50;
            fl_draw(std::to_string(label).c_str(), x() - 30, y() + h() - i);
            fl_line(x() - 5, y() + h() - i, x() + 5, y() + h() - i);
        }

        // Draw X-axis labels and ticks
        for (int i = 0; i <= 750; i += 50) {
            int label = 50 + (i / 50) * 50;
            fl_draw(std::to_string(label).c_str(), x() + i, y() + h() + 15);
            fl_line(x() + i, y() + h() - 5, x() + i, y() + h() + 5);
        }

        // Draw all shapes stored in the shapes vector
        for (auto& shape : shapes) {
            shape->draw();
        }
    }

    // Adds a new shape to the canvas
    void addShape(Shape* shape) { shapes.push_back(shape); }
    
    ~Canvas() {
        // Free allocated memory for shapes
        for (auto& shape : shapes) {
            delete shape;
        }
    }
};

// Input form for creating a Line
class LineInputForm : public Fl_Window {
private:
    Fl_Input* x1Input; // Input field for X1
    Fl_Input* y1Input; // Input field for Y1
    Fl_Input* x2Input; // Input field for X2
    Fl_Input* y2Input; // Input field for Y2
    Fl_Slider* thicknessSlider; // Slider to adjust line thickness
    Fl_Box* thicknessLabel;     // Label to display current thickness value
    Fl_Button* submitButton;    // Button to submit the form
    Canvas* canvas;             // Canvas where the line will be added

    // Callback to update the thickness label when the slider is moved
    static void thicknessSliderCallback(Fl_Widget* widget, void* data) {
        Fl_Slider* slider = static_cast<Fl_Slider*>(widget);
        Fl_Box* label = static_cast<Fl_Box*>(data);

        if (label == nullptr) {
            std::cerr << "thicknessLabel is null!" << std::endl;
            return;
        }

        int roundedValue = static_cast<int>(std::round(slider->value()));
        slider->value(roundedValue);

        std::string labelText = "Bold = " + std::to_string(roundedValue);
        label->copy_label(labelText.c_str());
    }

    // Callback when the submit button is clicked
    static void submitCallback(Fl_Widget* widget, void* data) {
        LineInputForm* form = static_cast<LineInputForm*>(data);
        form->onSubmit();
    }

    // Method to handle form submission
    void onSubmit() {
        int x1 = atoi(x1Input->value());
        int y1 = atoi(y1Input->value());
        int x2 = atoi(x2Input->value());
        int y2 = atoi(y2Input->value());
        int thickness = static_cast<int>(thicknessSlider->value());

        ColorfulPen* pen = new ColorfulPen();
        pen->setThickness(thickness);
        canvas->addShape(new Line(pen, x1, y1, x2, y2));
        canvas->redraw();
        this->hide(); // Close the form
    }

public:
    // Constructor to set up the input form
    LineInputForm(Canvas* c)
        : Fl_Window(400, 300, "Line Input"), canvas(c) {
        x1Input = new Fl_Input(100, 20, 200, 30, "X1:");
        y1Input = new Fl_Input(100, 60, 200, 30, "Y1:");
        x2Input = new Fl_Input(100, 100, 200, 30, "X2:");
        y2Input = new Fl_Input(100, 140, 200, 30, "Y2:");

        thicknessSlider = new Fl_Slider(100, 180, 200, 30);
        thicknessSlider->type(FL_HOR_NICE_SLIDER);
        thicknessSlider->bounds(1, 5);
        thicknessSlider->value(1);

        thicknessLabel = new Fl_Box(100, 220, 200, 30, "Bold = 1");
        thicknessLabel->box(FL_NO_BOX);
        thicknessLabel->align(FL_ALIGN_CENTER);

        thicknessSlider->callback(thicknessSliderCallback, thicknessLabel);

        submitButton = new Fl_Button(100, 260, 200, 30, "Submit");
        submitButton->callback(submitCallback, this);

        end();
    }
};




// Input form for creating a rectangle
class RectangleInputForm : public Fl_Window {
private:
    Fl_Input* xInput;          // Input field for the x-coordinate of the rectangle
    Fl_Input* yInput;          // Input field for the y-coordinate of the rectangle
    Fl_Input* widthInput;      // Input field for the width of the rectangle
    Fl_Input* heightInput;     // Input field for the height of the rectangle
    Fl_Slider* thicknessSlider; // Slider for adjusting the rectangle's border thickness
    Fl_Box* thicknessLabel;     // Label to display the current thickness value
    Fl_Button* submitButton;    // Submit button to create the rectangle
    Canvas* canvas;             // Canvas where the rectangle will be drawn

    // Callback to update the thickness label when the slider value changes
    static void thicknessSliderCallback(Fl_Widget* widget, void* data) {
        Fl_Slider* slider = static_cast<Fl_Slider*>(widget);
        Fl_Box* label = static_cast<Fl_Box*>(data);

        if (label == nullptr) {
            std::cerr << "thicknessLabel is null!" << std::endl;
            return;
        }

        int roundedValue = static_cast<int>(std::round(slider->value()));
        slider->value(roundedValue);

        std::string labelText = "Bold = " + std::to_string(roundedValue);
        label->copy_label(labelText.c_str());
    }

    // Callback for the submit button to create a rectangle on the canvas
    static void submitCallback(Fl_Widget* widget, void* data) {
        RectangleInputForm* form = static_cast<RectangleInputForm*>(data);
        form->onSubmit();
    }

    // Method to handle form submission and draw the rectangle
    void onSubmit() {
        int x = atoi(xInput->value());
        int y = atoi(yInput->value());
        int width = atoi(widthInput->value());
        int height = atoi(heightInput->value());
        int thickness = static_cast<int>(thicknessSlider->value());

        ColorfulPen* pen = new ColorfulPen(); // Use a colorful pen to draw the rectangle
        pen->setThickness(thickness);
        canvas->addShape(new Rectangle(pen, x, y, width, height));
        canvas->redraw(); // Redraw the canvas to include the new rectangle

        this->hide(); // Close the form after submission
    }

public:
    // Constructor to set up the rectangle input form
    RectangleInputForm(Canvas* c)
        : Fl_Window(400, 300, "Rectangle Input"), canvas(c) {
        xInput = new Fl_Input(100, 20, 200, 30, "X:");        // Input for x-coordinate
        yInput = new Fl_Input(100, 60, 200, 30, "Y:");        // Input for y-coordinate
        widthInput = new Fl_Input(100, 100, 200, 30, "Width:"); // Input for width
        heightInput = new Fl_Input(100, 140, 200, 30, "Height:"); // Input for height

        thicknessSlider = new Fl_Slider(100, 180, 200, 30);   // Slider for thickness
        thicknessSlider->type(FL_HOR_NICE_SLIDER);            // Horizontal slider
        thicknessSlider->bounds(1, 5);                        // Thickness range: 1-5
        thicknessSlider->value(1);                            // Default thickness

        thicknessLabel = new Fl_Box(100, 220, 200, 30, "Bold = 1"); // Display current thickness
        thicknessLabel->box(FL_NO_BOX);
        thicknessLabel->align(FL_ALIGN_CENTER);

        thicknessSlider->callback(thicknessSliderCallback, thicknessLabel);

        submitButton = new Fl_Button(100, 260, 200, 30, "Submit"); // Button to submit the form
        submitButton->callback(submitCallback, this);

        end(); // Finish the layout
    }
};

// Input form for creating a circle
class CircleInputForm : public Fl_Window {
private:
    Fl_Input* cxInput;          // Input field for the x-coordinate of the circle's center
    Fl_Input* cyInput;          // Input field for the y-coordinate of the circle's center
    Fl_Input* radiusInput;      // Input field for the radius of the circle
    Fl_Slider* thicknessSlider; // Slider for adjusting the circle's border thickness
    Fl_Box* thicknessLabel;     // Label to display the current thickness value
    Fl_Button* submitButton;    // Submit button to create the circle
    Canvas* canvas;             // Canvas where the circle will be drawn

    // Callback to update the thickness label when the slider value changes
    static void thicknessSliderCallback(Fl_Widget* widget, void* data) {
        Fl_Slider* slider = static_cast<Fl_Slider*>(widget);
        Fl_Box* label = static_cast<Fl_Box*>(data);

        if (label == nullptr) {
            std::cerr << "thicknessLabel is null!" << std::endl;
            return;
        }

        int roundedValue = static_cast<int>(std::round(slider->value()));
        slider->value(roundedValue);

        std::string labelText = "Bold = " + std::to_string(roundedValue);
        label->copy_label(labelText.c_str());
    }

    // Callback for the submit button to create a circle on the canvas
    static void submitCallback(Fl_Widget* widget, void* data) {
        CircleInputForm* form = static_cast<CircleInputForm*>(data);
        form->onSubmit();
    }

    // Method to handle form submission and draw the circle
    void onSubmit() {
        int cx = atoi(cxInput->value());
        int cy = atoi(cyInput->value());
        int radius = atoi(radiusInput->value());
        int thickness = static_cast<int>(thicknessSlider->value());

        ColorfulPen* pen = new ColorfulPen(); // Use a colorful pen to draw the circle
        pen->setThickness(thickness);
        canvas->addShape(new Circle(pen, cx, cy, radius));
        canvas->redraw(); // Redraw the canvas to include the new circle

        this->hide(); // Close the form after submission
    }

public:
    // Constructor to set up the circle input form
    CircleInputForm(Canvas* c)
        : Fl_Window(400, 270, "Circle Input"), canvas(c) {
        cxInput = new Fl_Input(100, 20, 200, 30, "Center X:");  // Input for center x-coordinate
        cyInput = new Fl_Input(100, 60, 200, 30, "Center Y:");  // Input for center y-coordinate
        radiusInput = new Fl_Input(100, 100, 200, 30, "Radius:"); // Input for radius

        thicknessSlider = new Fl_Slider(100, 140, 200, 30);   // Slider for thickness
        thicknessSlider->type(FL_HOR_NICE_SLIDER);            // Horizontal slider
        thicknessSlider->bounds(1, 5);                        // Thickness range: 1-5
        thicknessSlider->value(1);                            // Default thickness

        thicknessLabel = new Fl_Box(100, 180, 200, 30, "Bold = 1"); // Display current thickness
        thicknessLabel->box(FL_NO_BOX);
        thicknessLabel->align(FL_ALIGN_CENTER);

        thicknessSlider->callback(thicknessSliderCallback, thicknessLabel);

        submitButton = new Fl_Button(100, 220, 200, 30, "Submit"); // Button to submit the form
        submitButton->callback(submitCallback, this);

        end(); // Finish the layout
    }
};

// Callback for creating a static line using the LineInputForm
void staticLineCallback(Fl_Widget*, void* data) {
    Canvas* canvas = static_cast<Canvas*>(data);
    LineInputForm* form = new LineInputForm(canvas);
    form->show(); // Display the input form
}

// Callback for creating a static rectangle using the RectangleInputForm
void staticRectangleCallback(Fl_Widget*, void* data) {
    Canvas* canvas = static_cast<Canvas*>(data);
    RectangleInputForm* form = new RectangleInputForm(canvas);
    form->show(); // Display the input form
}

// Callback for creating a static circle using the CircleInputForm
void staticCircleCallback(Fl_Widget*, void* data) {
    Canvas* canvas = static_cast<Canvas*>(data);
    CircleInputForm* form = new CircleInputForm(canvas);
    form->show(); // Display the input form
}

// Callback for creating a random line with random coordinates and thickness
void randomLineCallback(Fl_Widget*, void* data) {
    static int thickness = 1; // Start with thickness 1, cycles from 1 to 5
    Canvas* canvas = static_cast<Canvas*>(data);
    ColorfulPen* pen = new ColorfulPen();
    pen->setThickness(thickness);

    // Generate random coordinates for the line's endpoints
    int x1 = 50 + rand() % 700;
    int y1 = 100 + rand() % 450;
    int x2 = 50 + rand() % 700;
    int y2 = 100 + rand() % 450;

    // Add the line to the canvas
    canvas->addShape(new Line(pen, x1, y1, x2, y2));
    canvas->redraw(); // Refresh the canvas to display the new line

    thickness = (thickness % 5) + 1; // Cycle thickness between 1 and 5
}

// Callback for creating a random rectangle with random dimensions and thickness
void randomRectangleCallback(Fl_Widget*, void* data) {
    static int thickness = 1; // Start with thickness 1, cycles from 1 to 5
    Canvas* canvas = static_cast<Canvas*>(data);
    ColorfulPen* pen = new ColorfulPen();
    pen->setThickness(thickness);

    // Generate random coordinates and dimensions for the rectangle
    int x = 50 + rand() % 650;
    int y = 100 + rand() % 400;
    int width = 50 + rand() % (750 - x);
    int height = 50 + rand() % (550 - y);

    // Add the rectangle to the canvas
    canvas->addShape(new Rectangle(pen, x, y, width, height));
    canvas->redraw(); // Refresh the canvas to display the new rectangle

    thickness = (thickness % 5) + 1; // Cycle thickness between 1 and 5
}

// Callback for creating a random circle with random center and radius
void randomCircleCallback(Fl_Widget*, void* data) {
    static int thickness = 1; // Start with thickness 1, cycles from 1 to 5
    Canvas* canvas = static_cast<Canvas*>(data);
    ColorfulPen* pen = new ColorfulPen();
    pen->setThickness(thickness);

    // Generate random coordinates for the circle's center and radius
    int radius = 20 + rand() % 50;
    int cx = 50 + radius + rand() % (700 - 2 * radius);
    int cy = 100 + radius + rand() % (450 - 2 * radius);

    // Add the circle to the canvas
    canvas->addShape(new Circle(pen, cx, cy, radius));
    canvas->redraw(); // Refresh the canvas to display the new circle

    thickness = (thickness % 5) + 1; // Cycle thickness between 1 and 5
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    // Create the main application window
    Fl_Window* window = new Fl_Window(800, 600, "Shape Drawer with FLTK");

    // Create a canvas for drawing shapes
    Canvas* canvas = new Canvas(50, 100, 700, 450);

    // Buttons for static shape creation
    Fl_Button* lineButton = new Fl_Button(50, 10, 150, 30, "Static Line");
    lineButton->callback(staticLineCallback, canvas);

    Fl_Button* rectangleButton = new Fl_Button(250, 10, 150, 30, "Static Rectangle");
    rectangleButton->callback(staticRectangleCallback, canvas);

    Fl_Button* circleButton = new Fl_Button(450, 10, 150, 30, "Static Circle");
    circleButton->callback(staticCircleCallback, canvas);

    // Buttons for random shape creation
    Fl_Button* randomLineButton = new Fl_Button(50, 50, 150, 30, "Random Line");
    randomLineButton->callback(randomLineCallback, canvas);

    Fl_Button* randomRectangleButton = new Fl_Button(250, 50, 150, 30, "Random Rectangle");
    randomRectangleButton->callback(randomRectangleCallback, canvas);

    Fl_Button* randomCircleButton = new Fl_Button(450, 50, 150, 30, "Random Circle");
    randomCircleButton->callback(randomCircleCallback, canvas);

    window->end();  // Finalize the layout of the window
    window->show(); // Show the window

    return Fl::run(); // Run the FLTK event loop
}

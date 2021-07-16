# Code Conventions used in NAS2D

As with any project, some coding conventions and stylistic guides should be adhered to when submitting code for review.

## Commenting & Documentation

Comments in code should take one of the following forms:

```cpp
// This is a comment

/**< This is a doxygen style comment */

/**
 * This is also a doxygen style comment
 */
```

Functions should use doxygen style comments. Formal parameters should be documented and any assumptions, notes or warnings should be noted.

```cpp
/**
 * Draws an image at a specified screen coordinate.
 *
 * \param img	Reference to an Image.
 * \param x		X-Coordinate
 * \param y		Y-Coordinate
 */
void drawImage(const Image& img, int x, int y)
{
	// in-block documentation
}
```

See [Doxygen Manual](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html) for details. If in doubt ask for assistance, we don't bite!

### Documentation Comments

Comments should be used only to document functions or to note assumptions that the code makes. Using comments to state what the code does only serves to clutter the code and make it harder to read. Worse still, over time the code may change but the comment may not be updated. Take for example:

```cpp
// get the country code
std::string country_code = get_country_code(SOME_INT_IDENTIFIER);

// if country code is US
if (country_code == "US")
{
    // display the form input for state
    std::cout << form_input_state() << std::endl;
}
```

The comments here only repeat what the code does and make it harder to read the code.

### Rules of Thumb

-   Comments should not restate what code does.
-   If a code block needs comments to explain what it does, it's too complex. Refactor it and make it readable.
-   Assumptions that the code makes should be noted.

## Indentation and Brace Placement

There are many different styles for this and everybody has an opinion but the most important thing is consistency.

All NAS2D code uses the following indentation and brace placements:

```cpp
void someFunction(int foo)
{
	// Fully expanded if/else if/else block
	if (foo == 1)
	{
		return;
	}
	else if (foo == 5)
	{
		std::cout << "foo == 5" << std::endl;
	}
	
	// Condensed if/else if/else block
	if (foo == 6) { return; }

	// For block
	for (int i = 0; i < 10; ++i)
	{
		// do something here.
	}
	
	// Switch control block.
	switch(foo)
	{
	case 2:
		std::cout << "foo == 2" << std::endl;
		break;
	case 3:
		std::cout << "foo == 3" << std::endl;
		break;
	default:
		break;
	}
	
	// While block
	int index = 0;
	while (index < 10)
	{
		++index;
	}
}
```

Please note that in all cases braces are used including for simple single line if/for/while blocks. While somewhat verbose this is intended to avoid common mistakes that are easy to overlook. For example:

```cpp
void somefunc(int val)
{
	if (val == 1)
		std::cout << "val == 1" << std::endl;
		return; // Everything beyond this line is never reached.
	if (val == 2)
	{
		std::cout << "val == 2" << std::endl;
		return;
	}
	if (val == 3)
	{
		std::cout << "val == 3" << std::endl;
		return;
	}
	
	std::cout << "Unexpected value." << std::endl;
}
```

This may seem like an obvious problem but it's known to occur in production code. For a recent high profile example, see [Apple's goto fail bug](https://en.wikipedia.org/wiki/Unreachable_code#goto_fail_bug).

## Naming Scheme

-   Class names are CamelCase with an uppercase first letter.
-   Function and class method names are camelCase with a lowercase first letter.
-   Class member variables always begin with a lowercase 'm' and are CamelCase afterward.

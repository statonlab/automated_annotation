!["Build"](https://travis-ci.org/statonlab/automated_annotation.svg?branch=master)

This module allows you to generate annotation reports for organisms. By generating
a table of available annotations per organism, you'll be able to see which
vocabularies/databases are missing. It'll also email a report each month.

## Installation

Download and place in your Drupal modules directory, then run:

```bash
drush en -y automated_annotation
```

## Configuration

Visit `admin/reports/automated-annotation/settings` to configure the module.

## Displaying Reports

Visit `admin/reports/automated-annotation/report` to view reports.

![Report Example](docs/aa_report.png)

## Sending Emails

This module provides a drush command to send a report to a list of email addresses.

```bash
drush annotations-check
```

You can add this line to your crontab to run this command once a month:

```bash
# Where /var/www/html is the path to Drupal installation
0 2 1 * * drush annotations-check --root=/var/www/html
```

The above line will run the report command once a month on the first day of the month at 2 am.

## License

This software is licensed under GPLv3.

*Copyright 2018 University of Tennessee Knoxville*

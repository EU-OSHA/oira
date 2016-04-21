29.05.2015 - Upgrade instructions
=================================

Background: Configure HTTPS for administrative purposes

1. Visit /admin/config/system/securepages
    * Enable Secure Pages: Enabled
    * Switch back to http pages when there are no matches: Checked
    * Non-secure Base URL: http://osha-corp.mainstrat.com
    * Secure Base URL: https://osha-corp.mainstrat.com



24.04.2015 - Upgrade instructions
==================================

Aside from the common update procedure described in the technical documentation, apply the following manual changes:

__BEFORE__ the update:

1. Edit [conf/config.json](conf/config.json) "variables" section and append the following variables (see [conf/config.template.json](config.template.json)):
  * varnish_version: 3 - Leave 3, currently supported version is Varnish 3
  * varnish_control_terminal: 127.0.0.1:6082 - Fill-in the __address:port__ of your Varnish server
  * varnish_control_key: "secret key" - Only if authentication is configured in Varnish
  * varnish_socket_timeout: 150 - A decent default for Varnish hosted on another machine

__UPDATE STEPS__:

  * cd /expert/osha/docroot
  * git checkout tags/2015_04_27
  * drush fr -y --force osha_workflow
  * drush cc all
  * drush osha_build –y

__AFTER__ the update:

1. Edit `sites/default/settings.php` and append the following lines at the end of the file:
```
# Varnish cache configuration
$conf['cache_backends'][] = 'sites/all/modules/contrib/varnish/varnish.cache.inc';
$conf['cache_class_cache_page'] = 'VarnishCache';
```
A default tested Varnish VCL configuration file has been provided in (conf/varnish-devel.vcl)[conf/varnish-devel.vcl]

2. Configure the binding passwords for LDAP connection.

a. password for read-only account by visiting /admin/config/people/ldap/servers/edit/osha and set the field *Password for non-anonymous search* (under section BINDING METHOD)
b. password for writable account by visiting /admin/config/people/ldap/servers/edit/osha-write and set the field *Password for non-anonymous search* (under section BINDING METHOD)


3. Setup Drupal CRON (currently disabled)
```
* * * * * wget -O - -q -t 1 http://osha-corp-staging.mainstrat.com/sites/all/modules/contrib/elysia_cron/cron.php?cron_key=CRON_KEY
```

where CRON_KEY is taken from `/admin/config/system/cron/settings` screen. Cron above will run on every 1 minute.

*Note*: The various cron tasks are scheduled to run differently, setting to one minute ensures task can run when scheduled.

4. Run the first LDAP synchronization (optional)
  * From command line (shows more details): in docroot, execute command `drush osha-ldap-sync`
  * From web interface: Go to `/admin/config/system/cron` and execute task `osha_authentication_cron`

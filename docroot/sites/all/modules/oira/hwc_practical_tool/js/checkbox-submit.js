(function($){
    Drupal.behaviors.osha_publication = {
        attach: function(context, settings) {
            var $form = $('#hwc-practical-tool-menu-tools-form');
            $form.find('input[name=sort]').click(function(){
                $form.submit();
            });
            $form.find('input[type=checkbox]').click(function(){
                var $container = $(this).closest('.form-checkboxes');
                if ($(this).val() != 0) {
                    $container.find('[value=0]').prop('checked', false);
                }
                else {
                    $container.find('[type=checkbox]').not(this).prop('checked', false);
                }
                if ($container.find(':checked').length == 0) {
                    $container.find('[value=0]').prop('checked', true);
                }
                $form.submit();
            });
        }
    }
})(jQuery);

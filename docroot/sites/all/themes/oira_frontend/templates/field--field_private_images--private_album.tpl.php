<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if (!$label_hidden): ?>
    <div class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</div>
  <?php endif; ?>
  <div class="field-items"<?php print $content_attributes; ?>>
    <?php foreach ($items as $delta => $item): ?>
      <?php if ($delta == 0) { ?>
        <div class="field-item col-md-12" <?php print $item_attributes[$delta]; ?>><?php print render($item); ?></div>
      <?php } else { ?>
        <div class="field-item col-md-4 col-sm-6 col-xs-12 <?php print ($delta - 1) % 3 ? '' : 'clearfix'; ?>"<?php print $item_attributes[$delta]; ?>><?php print render($item); ?></div>
      <?php } ?>
    <?php endforeach; ?>
  </div>
</div>

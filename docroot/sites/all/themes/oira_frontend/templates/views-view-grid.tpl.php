<?php

/**
 * @file
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 *
 * @ingroup views_templates
 */
?><?php dpm($column_classes); ?>
<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="<?php print $class; ?>"<?php print $attributes; ?>>
  <?php foreach ($rows as $row_number => $columns): ?>
    <?php $x = (int) (12 / $options['columns']); ?>
    <div class="row <?php if ($row_classes[$row_number]) { print $row_classes[$row_number];  } ?>">
      <?php foreach ($columns as $column_number => $item): ?>
        <div class="col-md-<?php print $x; ?> col-sm-6 col-xs-12 <?php if ($column_classes[$row_number][$column_number]) { print $column_classes[$row_number][$column_number];  } ?>">
          <div class="views-item-columns-container">
            <?php print $item; ?>
          </div>
        </div>
      <?php endforeach; ?>
    </div>
  <?php endforeach; ?>
</div>
